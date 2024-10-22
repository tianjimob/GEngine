#include "class.h"
#include "cursor.h"
#include "meta_reflection_data.h"

#include <clang-c/Index.h>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <inja.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// meta_parser.exe module_name project_dir gen_to_save_dir include_path
int main(int argc, char **argv) {
  namespace fs = std::filesystem;

  if (argc != 5) {
    std::cerr << "Num of args is not right !" << std::endl;
    exit(-1);
  }

  std::cout << "starting parse meta info ..." << std::endl;
  std::vector<const char *> arguments = {"-x",          "c++",
                                         "-std=c++17",  "-DNDEBUG",
                                         "-D__clang__", "-D__META_PARSER__",
                                         "-w",          "-MG",
                                         "-M",          "-o changeLog.txt"};

  fs::path projectDir = std::string{argv[2]};
  fs::path saveDir = std::string{argv[3]};
  fs::path includePath = std::string{argv[4]};
  std::string includeArg = std::string{"-I"} + includePath.string();
  fs::path refSaveDir = saveDir / "_generated" / "reflection";
  fs::path serSaveDir = saveDir / "_generated" / "serializer";
  fs::path workPath = fs::current_path();

  std::filesystem::remove_all(refSaveDir);
  std::filesystem::remove_all(serSaveDir);

  if (fs::exists(projectDir)) {
    arguments.emplace_back(includeArg.c_str());
    fs::create_directories(refSaveDir);
    fs::create_directories(serSaveDir);
  } else {
    std::cerr << projectDir << " is not exist!" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<fs::path> includes;
  for (const fs::directory_entry &dirEntry :
       fs::recursive_directory_iterator(projectDir)) {
    if (dirEntry.path().string().find(refSaveDir.string()) != std::string::npos)
      continue;
    if (dirEntry.path().string().find(serSaveDir.string()) != std::string::npos)
      continue;

    if (dirEntry.is_regular_file()) {
      auto &path = dirEntry.path();
      if (!path.has_extension())
        continue;
      auto extension = path.extension();
      if (extension == ".h" || extension == ".hpp")
        includes.emplace_back(fs::absolute(dirEntry.path()));
    }
  }

  auto index = clang_createIndex(0, 0);

  inja::Environment env;
  inja::Template tempMeta = env.parse_template(
      "src/engine/source/tool/meta_parser/template/meta_reflection.template");
  inja::Template tempCpp = env.parse_template(
      "src/engine/source/tool/meta_parser/template/gen_cpp.template");
  inja::Template tempSerializer = env.parse_template(
      "src/engine/source/tool/meta_parser/template/serializer.template");
  inja::Template tempSerializerCpp = env.parse_template(
      "src/engine/source/tool/meta_parser/template/serializer_cpp.template");

  struct RegisterEntry {
    std::string filename;
    std::string serializerFilename;
    std::vector<std::string> metaNames;
  };

  int count = 1;

  std::cout<<"start parsing meta reflection ..."<<std::endl;
  MetaReflectionData totalMetaData;
  std::unordered_map<std::string, std::vector<std::string>> includesMap;
  for (auto &include : includes) {
    includesMap.emplace(include.string(), std::vector<std::string>{});

    std::cout << "[ " << count++ << "/"
              << includes.size() << " ] " << "parsing file " << include  << std::endl;

    auto translator = clang_parseTranslationUnit(
        index, include.string().c_str(), arguments.data(),
        (int)arguments.size(), nullptr, 0, CXTranslationUnit_None);
    if (!translator) {
      std::cerr << "Failed to parse translation unit." << std::endl;
      exit(EXIT_FAILURE);
    }

    struct ClientData {
      ClientData(MetaReflectionData &metaData, const fs::path &path,
                 std::vector<std::string>
                     &registers)
          : metaData(metaData), path(path), registers(registers) {}
      MetaReflectionData& metaData;
      fs::path path;
      std::vector<std::string>& registers;
    };
    ClientData clientData{totalMetaData, include, includesMap[include.string()]};
    auto childVisitor = [](CXCursor cursor, CXCursor parent,
                           CXClientData data) {
      auto &clientData = *reinterpret_cast<ClientData *>(data);
      auto &metaData = clientData.metaData;
      auto &registers = clientData.registers;

      Cursor cur{cursor};
      fs::path filename = cur.getSourceFile();
      if (filename != clientData.path)
        return CXChildVisit_Continue;

      if (cur.getKind() == CXCursor_EnumDecl &&
          cur.hasAnnotateAttr("meta-enum")) {
        Enum e = cur;
        metaData.addEnum(e);
        registers.emplace_back(e.getEnumName());
        return CXChildVisit_Continue;
      } else if ((cur.getKind() == CXCursor_ClassDecl &&
                  cur.hasAnnotateAttr("meta-class")) ||
                 (cur.getKind() == CXCursor_StructDecl &&
                  cur.hasAnnotateAttr("meta-struct"))) {
        Class c = cur;
        metaData.addClass(c);
        registers.emplace_back(c.getClassName());
        return CXChildVisit_Continue;
      }

      return CXChildVisit_Recurse;
    };

    auto rootCursor = clang_getTranslationUnitCursor(translator);
    clang_visitChildren(rootCursor, childVisitor,
                        reinterpret_cast<CXClientData>(&clientData));

    clang_disposeTranslationUnit(translator);
  }
  totalMetaData.process();

  std::vector<RegisterEntry> entries;
  std::cout<<"start generating meta reflection ..."<<std::endl;
  count = 1;
  for (auto &include : includes) {
    std::cout << "[ " << count++ << "/" << includes.size() << " ] "
              << "generating file " << include << std::endl;

    fs::path includeHead;
    auto it1 = projectDir.begin();
    auto it2 = include.begin();
    for (; it1 != projectDir.end(); ++it1, ++it2) {
      if (it1->string() == it2->string())
        continue;
      else
        break;
    }
    for (; it2 != include.end(); ++it2) {
      includeHead /= *it2;
    }

    MetaReflectionData metaData{includeHead.string()};
    auto &registers = includesMap[include.string()];
    for (auto &registerItem : registers) {
      metaData.addRegister(totalMetaData.findRegisterByName(registerItem));
    }

    if (metaData.isEmpty()) continue;

    auto &entry = entries.emplace_back();
    entry.metaNames = metaData.getRegisterNamesList();

    const inja::json &data = metaData.getData();

    std::string filename = include.filename().string();
    auto pos = filename.find_last_of(".");
    filename.replace(pos, filename.size() - pos, ".gen.h");
    std::string filenameCpp = filename;
    filenameCpp.replace(pos, filename.size() - pos, ".gen.cpp");
    std::string filenameSerializer = filename;
    filenameSerializer.replace(pos, filename.size() - pos, ".serializer.gen.h");
    std::string filenameSerializerCpp = filename;
    filenameSerializerCpp.replace(pos, filename.size() - pos, ".serializer.gen.cpp");
    entry.filename = "_generated/reflection/" + filename;
    entry.serializerFilename = "_generated/serializer/" + filenameSerializer;
    std::ofstream of{refSaveDir / filename};
    std::ofstream ofCpp{refSaveDir / filenameCpp};
    std::ofstream ofSerializer{serSaveDir / filenameSerializer};
    std::ofstream ofSerializerCpp{serSaveDir / filenameSerializerCpp};
    try {
      env.render_to(of, tempMeta, data);
      env.render_to(ofCpp, tempCpp, data);
      env.render_to(ofSerializer, tempSerializer, data);
      env.render_to(ofSerializerCpp, tempSerializerCpp, data);
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  clang_disposeIndex(index);

  {
    inja::Template tempAutoRegister = env.parse_template(
        "src/engine/source/tool/meta_parser/template/auto_register.template");
    inja::Template tempAllSerializer =
        env.parse_template("src/engine/source/tool/meta_parser/template/all_serializer.template");
    
    inja::json allReflection;
    allReflection["reflections"] = inja::json::array();
    auto &reflections = allReflection["reflections"];
    for (auto &entry : entries) {
      auto &reflection = reflections.emplace_back();
      reflection["filename"] = entry.filename;
      reflection["serializer_filename"] = entry.serializerFilename;
      reflection["meta_names"] = inja::json::array();
      for (auto &s : entry.metaNames) {
        auto namespacePos = s.rfind("::");
        auto nameWithoutNamespace = s.substr(namespacePos + 2);
        reflection["meta_names"].emplace_back(nameWithoutNamespace);
      }
    }
    std::ofstream of{refSaveDir / "all_reflection.cpp"};
    std::ofstream ofSerializer{serSaveDir / "all_serializer.h"};
    try {
      env.render_to(of, tempAutoRegister, allReflection);
      env.render_to(ofSerializer, tempAllSerializer, allReflection);
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

}
