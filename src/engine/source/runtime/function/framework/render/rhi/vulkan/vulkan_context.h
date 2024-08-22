#pragma once

#include <memory>
#include <unordered_map>

#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_pipeline_state.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"


namespace GEngine {

class VulkanRHI;
class VulkanDevice;
class VulkanQueue;
class VulkanCommandBufferManager;

class VulkanRHICommandContext : public RHICommandContext {
 public:
  VulkanRHICommandContext(VulkanRHI *rhi, VulkanDevice *device,
                          VulkanQueue *queue, bool isImmediate);
  VulkanQueue *getQueue() { return m_queue; }

  virtual void RHIDispatchComputeShader(uint32_t groupCountX,
                                        uint32_t groupCountY,
                                        uint32_t groupCountZ) override;
  virtual void RHISetComputePipelineState(
      std::shared_ptr<RHIComputePipelineState> &computePipelineState,
      const void *parametersData) override;

  virtual void RHIDrawIndexedPrimitive(
      std::shared_ptr<RHIBuffer> &IndexBufferRHI, int32_t BaseVertexIndex,
      uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex,
      uint32_t NumPrimitives, uint32_t NumInstances) override;
  virtual void RHISetGraphicsPipelineState(
      std::shared_ptr<RHIGraphicsPipelineState> &graphicsPipelineState,
      const void *parametersData) override;

  virtual void RHICopyBuffer(std::shared_ptr<RHIBuffer>& srcBuffer, std::shared_ptr<RHIBuffer>& dstBuffer) override;

  void waitIdle();

 private:
  VulkanRHI *m_rhi;
  VulkanDevice *m_device;
  VulkanQueue *m_queue;
  bool m_isImmediate;

  std::shared_ptr<VulkanCommandBufferManager> m_commandBufferManager;

  std::unordered_map<std::shared_ptr<VulkanRHIComputePipelineState>,
                     std::shared_ptr<VulkanComputePipelineDescriptorState>>
      m_computeStateMap;
  std::unordered_map<std::shared_ptr<VulkanRHIGraphicsPipelineState>,
                     std::shared_ptr<VulkanGraphicsPipelineDescriptorState>>
      m_graphicsStateMap;
  std::shared_ptr<VulkanComputePipelineDescriptorState> m_currentComputeState;
  std::shared_ptr<VulkanGraphicsPipelineDescriptorState> m_currentGraphicsState;
};

}  // namespace GEngine