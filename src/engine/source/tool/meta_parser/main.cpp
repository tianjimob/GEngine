#include "cursor.h"
#include "meta_reflection_data.h"
#include <clang-c/Index.h>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <inja.hpp>
#include <iomanip>
#include <iostream>
#include <string>
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
  fs::path workPath = fs::current_path();

  if (fs::exists(projectDir)) {
    arguments.emplace_back(includeArg.c_str());
    fs::create_directories(refSaveDir);
  } else {
    std::cerr << projectDir << " is not exist!" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<fs::path> includes;
  for (const fs::directory_entry &dirEntry :
       fs::recursive_directory_iterator(projectDir)) {
    if (dirEntry.path().string().find(refSaveDir.string()) != std::string::npos)
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

  // {
  //   std::ofstream metaHead{workPath / "meta_head.h"};
  //   for (auto &include : includes) {
  //     metaHead << "#include " << std::quoted(include.string()) << '\n';
  //   }
  // }

  auto index = clang_createIndex(0, 0);

  inja::Environment env;
  inja::Template tempMeta = env.parse_template(
      "src/engine/source/tool/meta_parser/meta_reflection.template");
  inja::Template tempCpp = env.parse_template("src/engine/source/tool/meta_parser/gen_cpp.template");

  struct RegisterEntry {
    std::string filename;
    std::vector<std::string> metaNames;
  };

  std::vector<RegisterEntry> entries;
  std::cout<<"start generating meta reflection ..."<<std::endl;
  for (auto &include : includes) {
    std::cout << "parsing file " << include << std::endl;

    auto translator = clang_parseTranslationUnit(
        index, include.string().c_str(), arguments.data(),
        (int)arguments.size(), nullptr, 0, CXTranslationUnit_None);
    if (!translator) {
      std::cerr << "Failed to parse translation unit." << std::endl;
      exit(EXIT_FAILURE);
    }

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

    struct ClientData {
      ClientData(const std::string &headname, const fs::path &path)
          : metaData(headname), path(path) {}
      MetaReflectionData metaData;
      fs::path path;
    };
    ClientData clientData{includeHead.string(), include};
    auto childVisitor = [](CXCursor cursor, CXCursor parent,
                           CXClientData data) {
      auto &clientData = *reinterpret_cast<ClientData *>(data);
      auto &metaData = clientData.metaData;

      Cursor cur{cursor};
      fs::path filename = cur.getSourceFile();
      if (filename != clientData.path)
        return CXChildVisit_Continue;

      if (cur.getKind() == CXCursor_EnumDecl &&
          cur.hasAnnotateAttr("meta-enum")) {
        metaData.addEnum(cur);
        return CXChildVisit_Continue;
      } else if (cur.getKind() == CXCursor_ClassDecl &&
                 cur.hasAnnotateAttr("meta-class")) {
        metaData.addClass(cur);
        return CXChildVisit_Continue;
      }

      return CXChildVisit_Recurse;
    };

    auto rootCursor = clang_getTranslationUnitCursor(translator);
    clang_visitChildren(rootCursor, childVisitor,
                        reinterpret_cast<CXClientData>(&clientData));

    clang_disposeTranslationUnit(translator);

    if (clientData.metaData.isEmpty())
      continue;

    auto &entry = entries.emplace_back();
    entry.metaNames = clientData.metaData.getRegisterNamesList();

    const inja::json &data = clientData.metaData.getData();

    std::string filename = include.filename().string();
    auto pos = filename.find_last_of(".");
    filename.replace(pos, filename.size() - pos, ".gen.h");
    std::string filenameCpp = filename;
    filenameCpp.replace(pos, filename.size() - pos, ".gen.cpp");
    entry.filename = "_generated/reflection/" + filename;
    std::ofstream of{refSaveDir / filename};
    std::ofstream ofCpp{refSaveDir / filenameCpp};
    try {
      env.render_to(of, tempMeta, data);
      env.render_to(ofCpp, tempCpp, data);
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
  clang_disposeIndex(index);

  {
    inja::Template tempAutoRegister =
        env.parse_template("src/engine/source/tool/meta_parser/auto_register.template");
    
    inja::json allReflection;
    allReflection["reflections"] = inja::json::array();
    auto &reflections = allReflection["reflections"];
    for (auto &entry : entries) {
      auto &reflection = reflections.emplace_back();
      reflection["filename"] = entry.filename;
      reflection["meta_names"] = inja::json::array();
      for (auto &s : entry.metaNames) {
        auto namespacePos = s.rfind("::");
        auto nameWithoutNamespace = s.substr(namespacePos + 2);
        reflection["meta_names"].emplace_back(nameWithoutNamespace);
      }
    }
    std::ofstream of{refSaveDir / "all_reflection.cpp"};
    try {
      env.render_to(of, tempAutoRegister, allReflection);
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

}