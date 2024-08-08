#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>
#include <vector>
namespace GEngine {

enum class VertexElementType {
  None,
  Float1,
  Float2,
  Float3,
  Float4,
};

struct VertexStream {
  std::shared_ptr<RHIBuffer> vertexBuffer = nullptr;
  uint32_t stride = 0;
  uint32_t offset = 0;
  VertexElementType elementType = VertexElementType::None;
};

class VertexStreams {
public:
  void addVertexStream(VertexStream &vertexStream) {
    m_vertexStreams.emplace_back(vertexStream);
  }
private:
  std::vector<VertexStream> m_vertexStreams;
};

}