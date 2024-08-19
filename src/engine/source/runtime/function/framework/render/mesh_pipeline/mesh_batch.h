#pragma once

#include "function/framework/render/rhi/rhi_type.h"
#include <memory>
namespace GEngine {

class VertexStreams;
class Material;

struct MeshBatch {
  const VertexStreams* vertexStreams;
  RHIPrimitiveType type;

  std::shared_ptr<Material> material;
};

}