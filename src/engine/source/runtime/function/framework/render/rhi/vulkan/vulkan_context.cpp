#include "vulkan_context.h"

#include <cassert>
#include <memory>
#include <vector>

#include "function/framework/render/rhi/vulkan/vulkan_pipeline_state.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_command_buffer.h"

namespace GEngine {

VulkanRHICommandContext::VulkanRHICommandContext(VulkanDynamicRHI *rhi,
                                                 VulkanDevice *device,
                                                 VulkanQueue *queue,
                                                 bool isImmediate)
    : m_rhi(rhi), m_device(device), m_queue(queue), m_isImmediate(isImmediate) {
  m_commandBufferManager =
      std::make_shared<VulkanCommandBufferManager>(m_device, this);
  m_commandBufferManager->init();
  if (m_isImmediate) {
    m_commandBufferManager->submitActiveCommandBuffer(
        std::vector<std::shared_ptr<VulkanSemaphore>>{});
    m_commandBufferManager->prepareForNewActiveCommandBuffer();
  }
}

void VulkanRHICommandContext::RHIDispatchComputeShader(uint32_t groupCountX,
                                                       uint32_t groupCountY,
                                                       uint32_t groupCountZ) {
  std::shared_ptr<VulkanCommandBuffer> cmdBuffer =
      m_commandBufferManager->getActiveCommandBuffer();
  assert(!cmdBuffer->isInsideRenderPass());

  VkCommandBuffer cmd = cmdBuffer->getCommandBuffer();
  m_currentComputeState->bind(cmd);
  vkCmdDispatch(cmd, groupCountX, groupCountY, groupCountZ);
  
  m_commandBufferManager->submitActiveCommandBuffer(
      std::vector<std::shared_ptr<VulkanSemaphore>>{});
  m_commandBufferManager->prepareForNewActiveCommandBuffer();
}

void VulkanRHICommandContext::RHISetComputePipelineState(
    std::shared_ptr<RHIComputePipelineState> &computePipelineState, const void* parametersData) {
  std::shared_ptr<VulkanRHIComputePipelineState> vulkanState =
      std::static_pointer_cast<VulkanRHIComputePipelineState>(
          computePipelineState);
  if (auto it = m_computeStateMap.find(vulkanState);
      it != m_computeStateMap.end()) {
    m_currentComputeState = it->second;
  } else {
    m_currentComputeState =
        std::make_shared<VulkanComputePipelineDescriptorState>(vulkanState, parametersData);
    m_computeStateMap[vulkanState] = m_currentComputeState;
  }
}

}  // namespace GEngine