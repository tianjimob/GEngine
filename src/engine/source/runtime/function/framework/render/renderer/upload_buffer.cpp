#include "upload_buffer.h"
#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/compute_shader_utils.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/rhi_shader.h"
#include "function/framework/render/rhi/rhi_type.h"
#include <memory>

namespace GEngine {

void UploadBuffer::begin(std::shared_ptr<RHIBuffer> &dstBuffer,
                         uint32_t numElements, uint32_t numBytesPerElement) {
  if (!m_uploadBufferRHI) {
    m_uploadBufferRHI = GlobalRHI->createBuffer(
        numElements * numBytesPerElement, RHIBufferUsageFlags::StorageBuffer,
        RHIMemoryPropertyFlags::HostVisibleAndCoherent);
    m_numElements = numElements;
    m_numBytesPerElement = numBytesPerElement;
    GlobalRHI->mapBuffer(m_data, m_uploadBufferRHI);
  }
}

template <>
void UploadBuffer::end<PrimitiveSceneCS::Parameters>(
    std::shared_ptr<RHIBuffer> &dstBuffer) {
  GlobalRHI->unmapBuffer(m_uploadBufferRHI);
  PrimitiveSceneCS::Parameters params;
  params.updateSize = m_numElements;
  params.ssboSrc = m_uploadBufferRHI.get();
  params.ssboDst = dstBuffer.get();

  std::shared_ptr<RHIComputeShader> memcpyCS =
      std::static_pointer_cast<RHIComputeShader>(
          RHIShaders::getShader("PrimitiveSceneCS"));

  ComputeShaderUtils::dispatch(
        memcpyCS, params, (params.updateSize + (64 - 1)) / 64, 1, 1);
}

}