#include "obj_importer.h"

namespace GEngine {

std::shared_ptr<StaticMesh> ObjImporter::importStaticMesh(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes) {
  std::shared_ptr<StaticMesh> staticMesh = std::make_shared<StaticMesh>();
  auto &staticMeshModel = staticMesh->addMeshModel();
  auto *meshDescriptor = staticMeshModel->getMeshDescriptor();

  auto& vertices = attrib.vertices;
  for (int i = 0; i < vertices.size() / 3; ++i) {
    meshDescriptor->m_vertexPositions.emplace_back(
        vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
  }
  return staticMesh;
}
}