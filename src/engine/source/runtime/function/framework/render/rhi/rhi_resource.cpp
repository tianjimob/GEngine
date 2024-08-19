#include "rhi_resource.h"
#include "CRC.h"
#include <cstdint>
#include <vector>

namespace GEngine {

uint32_t RHIGraphicsPipelineStateInitiazlier::getRenderPassHash() const {
  std::vector<uint32_t> fullHashInfo;
  for (auto format : colorAttachmentFormats) {
    fullHashInfo.emplace_back(static_cast<uint32_t>(format));
  }
  fullHashInfo.emplace_back(static_cast<uint32_t>(hasDepthAttachment));
  fullHashInfo.emplace_back(static_cast<uint32_t>(depthAttachmentFormat));
  fullHashInfo.emplace_back(static_cast<uint32_t>(depthLoadOp));
  fullHashInfo.emplace_back(static_cast<uint32_t>(depthStoreOp));
  fullHashInfo.emplace_back(static_cast<uint32_t>(stencilLoadOp));
  fullHashInfo.emplace_back(static_cast<uint32_t>(stencilStoreOp));
  fullHashInfo.emplace_back(static_cast<uint32_t>(subpassType));
  
  return CRC::Calculate(fullHashInfo.data(),
                        sizeof(uint32_t) * fullHashInfo.size(), CRC::CRC_32());
}

}