#include "upload_buffer.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_type.h"

namespace GEngine {

void UploadBuffer::begin(std::shared_ptr<RHIBuffer> &dstBuffer,
                         uint32_t numElements, uint32_t numBytesPerElement) {
  if (!m_uploadBufferRHI) {
    m_uploadBufferRHI = GlobalRHI->createBuffer(
        numElements * numBytesPerElement, RHIBufferUsageFlags::StorageBuffer,
        RHIMemoryPropertyFlags::HostVisibleAndCoherent);
    m_numBytesPerElement = numBytesPerElement;
    GlobalRHI->mapBuffer(m_data, m_uploadBufferRHI);
  }
}

void UploadBuffer::end(std::shared_ptr<RHIBuffer> &dstBuffer) {
  
}

}