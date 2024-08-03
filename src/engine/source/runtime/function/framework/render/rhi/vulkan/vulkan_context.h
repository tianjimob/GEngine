#pragma once

#include <memory>
#include <unordered_map>

#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_pipeline_state.h"


namespace GEngine {

class VulkanDynamicRHI;
class VulkanDevice;
class VulkanQueue;
class VulkanCommandBufferManager;

class VulkanRHICommandContext : public RHICommandContext {
 public:
  VulkanRHICommandContext(VulkanDynamicRHI *rhi, VulkanDevice *device,
                          VulkanQueue *queue, bool isImmediate);
  VulkanQueue *getQueue() { return m_queue; }

  virtual void RHIDispatchComputeShader(uint32_t groupCountX,
                                        uint32_t groupCountY,
                                        uint32_t groupCountZ) override;
  virtual void RHISetComputePipelineState(
      std::shared_ptr<RHIComputePipelineState> &computePipelineState, const void* parametersData)
      override;

 private:
  VulkanDynamicRHI *m_rhi;
  VulkanDevice *m_device;
  VulkanQueue *m_queue;
  bool m_isImmediate;

  std::shared_ptr<VulkanCommandBufferManager> m_commandBufferManager;

  std::unordered_map<std::shared_ptr<VulkanRHIComputePipelineState>,
                     std::shared_ptr<VulkanComputePipelineDescriptorState>>
      m_computeStateMap;
  std::shared_ptr<VulkanComputePipelineDescriptorState> m_currentComputeState;
};

}  // namespace GEngine