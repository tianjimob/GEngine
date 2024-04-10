#pragma once

#include <memory>
namespace GEngine {

class RHICommandContext {};

class RHICommandBufferImmediate {};

class RHICommandBufferExecutor {
 public:
  RHICommandBufferImmediate &getCommandBufferImmediate() {
    return m_commandBufferImmediate;
  }

private:
  RHICommandBufferImmediate m_commandBufferImmediate;
};

class RHIImage {};

struct RHIGraphicsPipelineState {};

using RHIGraphicsPipelineStateRef = std::shared_ptr<RHIGraphicsPipelineState>;

class RHIGraphicsPipelineStateCreateInfo {
 public:
 private:
};

}  // namespace GEngine