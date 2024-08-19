#pragma once

#include "function/framework/render/mesh_pipeline/vertex_streams.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>

namespace GEngine {

class RHIBuffer;

struct MeshDrawCommand {
  VertexStreams vertexStreams;
  std::shared_ptr<RHIBuffer> indexBuffer;

  // todo: pso
  RHIGraphicsPipelineStateInitiazlier pipelineState;

  // draw command parameters
  uint32_t firstIndex;
  uint32_t numPrimitives;
  uint32_t numInstances;

  union {
    struct {
      uint32_t baseVertexIndex;
      uint32_t numVertices;
    } VertexParams;
    struct {
      std::shared_ptr<RHIBuffer> indirectBuffer;
      uint32_t offset;
    } IndirectArgs;
  };

  
};

}