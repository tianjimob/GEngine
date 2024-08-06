#include "static_mesh_manager.h"
#include "core/log/logger.h"
#include "function/framework/component/mesh/importer/fbx_importer.h"
#include "function/framework/component/mesh/importer/obj_importer.h"
#include <filesystem>

namespace GEngine {

static DECLARE_LOG_CATEGORY(LogGameEngine);

std::unordered_map<std::string, std::shared_ptr<StaticMesh>> StaticMeshManager::m_staticMeshMap;

std::shared_ptr<StaticMesh>
StaticMeshManager::loadStaticMesh(const std::string &staticMeshPath) {
  if (auto it = m_staticMeshMap.find(staticMeshPath);
      it != m_staticMeshMap.end()) {
    return it->second;
  } else {
    std::filesystem::path path = staticMeshPath;
    auto ext = path.extension();
    if (ext == ".fbx") {
      ufbx_error error;
      ufbx_scene *scene = ufbx_load_file(staticMeshPath.c_str(), NULL, &error);
      if (!scene) {
        LOG_ERROR(LogGameEngine, "Failed to load scene: %s", error.description.data);
        return nullptr;
      }
      m_staticMeshMap[staticMeshPath] = FbxImporter::importStaticMesh(scene->nodes);
    } else if (ext == ".obj") {
      tinyobj::ObjReaderConfig readerConfig;  // Path to material files
      tinyobj::ObjReader reader;

      if (!reader.ParseFromFile(staticMeshPath, readerConfig)) {
        if (!reader.Error().empty()) {
          LOG_ERROR(LogGameEngine, "TinyObjReader: %s", reader.Error());
        }
        return nullptr;
      }

      if (!reader.Warning().empty()) {
        LOG_WARN(LogGameEngine, "TinyObjReader: %s", reader.Warning());
      }

      m_staticMeshMap[staticMeshPath] = ObjImporter::importStaticMesh(reader.GetAttrib(), reader.GetShapes());
    }

    return m_staticMeshMap[staticMeshPath];
  }
  
}

}