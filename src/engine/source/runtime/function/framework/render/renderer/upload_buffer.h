#pragma once

#include "core/math/vector4.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include <cstdint>
#include <memory>
namespace GEngine {

class UploadBuffer {
public:
  void begin(std::shared_ptr<RHIBuffer> &dstBuffer, uint32_t numElements,
             uint32_t numBytesPerElement);
  void end(std::shared_ptr<RHIBuffer> &dstBuffer);
  void* getMappedVector4(uint32_t index) {
    return reinterpret_cast<uint8_t*>(m_data) + index * m_numBytesPerElement;
  }

private:
  std::shared_ptr<RHIBuffer> m_uploadBufferRHI;
  uint32_t m_numBytesPerElement;
  void* m_data;
};

}