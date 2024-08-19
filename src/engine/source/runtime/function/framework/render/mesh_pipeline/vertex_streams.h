#pragma once

#include <memory>
#include <vector>
namespace GEngine {

class RHIBuffer;

enum class VertexElementType {
  None,
  Float1 = 0,
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

  const std::vector<VertexStream>& getVertexStreams() const { return m_vertexStreams; }
private:
  std::vector<VertexStream> m_vertexStreams;
};

}