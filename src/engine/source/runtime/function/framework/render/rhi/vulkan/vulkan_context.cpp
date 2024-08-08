#include "vulkan_context.h"

#include <cassert>
#include <memory>
#include <vector>

#include "function/framework/render/rhi/vulkan/vulkan_pipeline_state.h"
#include "function/framework/render/rhi/vulkan/vulkan_queue.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_command_buffer.h"

namespace GEngine {

VulkanRHICommandContext::VulkanRHICommandContext(VulkanRHI *rhi,
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
        std::make_shared<VulkanComputePipelineDescriptorState>(m_device, vulkanState, parametersData);
    m_computeStateMap[vulkanState] = m_currentComputeState;
  }
}

void VulkanRHICommandContext::RHICopyBuffer(
    std::shared_ptr<RHIBuffer> &srcBuffer,
    std::shared_ptr<RHIBuffer> &dstBuffer) {
  assert(srcBuffer->size() == dstBuffer->size());

  std::shared_ptr<VulkanCommandBuffer> cmdBuffer =
      m_commandBufferManager->getActiveCommandBuffer();
  VkCommandBuffer cmd = cmdBuffer->getCommandBuffer();

  VkBufferCopy copyRegion{};
  copyRegion.size = srcBuffer->size();
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  vkCmdCopyBuffer(cmd, (VkBuffer)srcBuffer->getHandle(),
                  (VkBuffer)dstBuffer->getHandle(), 1, &copyRegion);

  m_commandBufferManager->submitActiveCommandBuffer(
      std::vector<std::shared_ptr<VulkanSemaphore>>{});
  m_queue->waitIdle();
  m_commandBufferManager->prepareForNewActiveCommandBuffer();
}

void VulkanRHICommandContext::waitIdle() {
  m_queue->waitIdle();
}

}  // namespace GEngine