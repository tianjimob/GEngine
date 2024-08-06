#pragma once

#include <memory>
#include <vector>

#include "core/math/box_sphere_bounds.h"
#include "core/reflection/reflection.h"
#include "function/framework/component/mesh/mesh_descriptor.h"
#include "function/framework/component/mesh/vertex_streams.h"
#include "function/framework/object/object.h"
#include "function/framework/render/rhi/rhi_resource.h"


namespace GEngine {

class StaticMeshModel {
 public:
  MeshDescriptor *getMeshDescriptor() {
    if (m_meshDesciptor == nullptr) {
      m_meshDesciptor = std::make_unique<MeshDescriptor>();
    }
    return m_meshDesciptor.get();
  }

  void build();

  BoxSphereBounds computeBoundsFromPositionBuffer();

 public:
 private:
  std::unique_ptr<MeshDescriptor> m_meshDesciptor;

  std::shared_ptr<RHIBuffer> m_positionBuffer;
  std::shared_ptr<RHIBuffer> m_normalBuffer;
  std::shared_ptr<RHIBuffer> m_tangentBuffer;
  std::shared_ptr<RHIBuffer> m_colorBuffer;
  std::shared_ptr<RHIBuffer> m_uvBuffer;

  VertexStreams m_vertexStreams;

 private:
};

class StaticMesh {
 public:
  std::shared_ptr<StaticMeshModel> &addMeshModel() {
    return m_staticMeshModels.emplace_back(std::make_shared<StaticMeshModel>());
  }

  void build() {
    m_bounds = m_staticMeshModels.front()->computeBoundsFromPositionBuffer();
  }

 private:
  std::vector<std::shared_ptr<StaticMeshModel>> m_staticMeshModels; // LOD
  BoxSphereBounds m_bounds;
};

}  // namespace GEngine