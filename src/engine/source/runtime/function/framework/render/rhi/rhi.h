#pragma once

#include "function/framework/render/rhi/rhi_type.h"
#include "rhi_resource.h"
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace GEngine {

class RHI {
public:
  virtual void init() = 0;
  virtual std::string_view getName() = 0;

  virtual RHIGraphicsPipelineStateRef createGraphicsPipelineState(
      const RHIGraphicsPipelineStateCreateInfo &createInfo) = 0;
  virtual std::shared_ptr<RHIComputePipelineState> createComputePipelineState(std::shared_ptr<RHIComputeShader>& computeShader) = 0;

  virtual std::shared_ptr<RHIUniformBuffer> createUniformBuffer(uint32_t size) = 0;
  virtual std::shared_ptr<RHIBuffer>
  createBuffer(uint32_t size, RHIBufferUsageFlags usage,
               RHIMemoryPropertyFlags property) = 0;
  virtual std::shared_ptr<RHIComputeShader> createComputeShader(const std::vector<uint8_t>& shaderCode) = 0;
};

extern RHI *GlobalRHI;

}