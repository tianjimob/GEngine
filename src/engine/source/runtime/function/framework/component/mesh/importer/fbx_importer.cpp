#include "fbx_importer.h"
#include "function/framework/component/mesh/static_mesh.h"
#include "ufbx/ufbx.h"
#include <memory>

namespace GEngine {

std::shared_ptr<StaticMesh>
FbxImporter::importStaticMesh(ufbx_node_list &nodes) {
  std::shared_ptr<StaticMesh> staticMesh = std::make_shared<StaticMesh>();
  auto &staticMeshModel = staticMesh->addMeshModel();
  auto *meshDescriptor = staticMeshModel->getMeshDescriptor();
  int vertexCount = 0;
  int indexCount = 0;
  for (auto &node : nodes) {
    if (node->mesh) {
      vertexCount += node->mesh->num_vertices;
      indexCount += node->mesh->num_indices;
    }
  }
  meshDescriptor->m_vertexPositions.reserve(vertexCount);
  meshDescriptor->m_vertexNormals.reserve(vertexCount);
  meshDescriptor->m_vertexTangents.reserve(vertexCount);
  meshDescriptor->m_vertexColors.reserve(vertexCount);
  meshDescriptor->m_vertexUVs.reserve(vertexCount);
  meshDescriptor->m_indices.reserve(indexCount);
  int currCount = 0;
  for (auto &node : nodes) {
    if (node->mesh) {
      auto &mesh = *(node->mesh);
      auto &localToWorld = node->node_to_world;
      
      for (int vertexIndex = 0; vertexIndex < mesh.num_vertices;
           ++vertexIndex) {
        ufbx_vec3 position = mesh.vertex_position[vertexIndex];
        position = ufbx_transform_position(&localToWorld, position);
        meshDescriptor->m_vertexPositions.emplace_back(position.x, position.y,
                                                       position.z);
      }

      if (mesh.vertex_normal.exists) {
        for (int vertexIndex = 0; vertexIndex < mesh.num_vertices;
             ++vertexIndex) {
          ufbx_vec3 &normal = mesh.vertex_normal[vertexIndex];
          meshDescriptor->m_vertexNormals.emplace_back(normal.x, normal.y,
                                                       normal.z);
        }
      }

      if (mesh.vertex_tangent.exists) {
        for (int vertexIndex = 0; vertexIndex < mesh.num_vertices;
             ++vertexIndex) {
          ufbx_vec3 &tangent = mesh.vertex_tangent[vertexIndex];
          meshDescriptor->m_vertexTangents.emplace_back(tangent.x, tangent.y,
                                                        tangent.z);
        }
      }

      if (mesh.vertex_color.exists) {
        for (int vertexIndex = 0; vertexIndex < mesh.num_vertices;
             ++vertexIndex) {
          ufbx_vec4 &color = mesh.vertex_color[vertexIndex];
          meshDescriptor->m_vertexColors.emplace_back(color.x, color.y, color.z,
                                                      color.w);
        }
      }

      if (mesh.vertex_uv.exists) {
        for (int vertexIndex = 0; vertexIndex < mesh.num_vertices;
             ++vertexIndex) {
          ufbx_vec2 &uv = mesh.vertex_uv[vertexIndex];
          meshDescriptor->m_vertexUVs.emplace_back(uv.x, uv.y);
        }
      }

      for (int indexIndex = 0; indexIndex < mesh.num_indices; ++indexIndex) {
        meshDescriptor->m_indices.emplace_back(mesh.vertex_indices[indexIndex] +
                                               currCount);
      }
      currCount += mesh.num_vertices;
    }
  }
  
  staticMeshModel->build();
  staticMesh->build();
  return staticMesh;
}

}