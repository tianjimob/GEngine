#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>
#include <vector>
namespace GEngine {

struct VertexStream {
  std::shared_ptr<RHIBuffer> vertexBuffer = nullptr;
  uint32_t stride = 0;
  uint32_t offset = 0;
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