#include "vulkan_queue.h"

#include "core/log/logger.h"
#include "function/framework/render/rhi/vulkan/vulkan_macros.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_command_buffer.h"
#include "vulkan_device.h"
#include "vulkan_sync.h"
#include <cassert>


namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VulkanQueue::VulkanQueue(VulkanDevice &device, uint32_t queueFamilyIndex)
    : m_device(device), m_queueFamilyIndex(queueFamilyIndex), m_queueIndex(0) {
  vkGetDeviceQueue(device.getDevice(), queueFamilyIndex, m_queueIndex,
                   &m_queue);
}

void VulkanQueue::submit(VulkanCommandBuffer &commandBuffer,
                         uint32_t nSemaphores, VkSemaphore *semaphores) {
  const VkCommandBuffer commandBuffers[]{commandBuffer.getCommandBuffer()};
  VkSubmitInfo submitInfo;
  ZERO_VULKAN_STRUCT(submitInfo);
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = nullptr;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = commandBuffers;
  submitInfo.signalSemaphoreCount = nSemaphores;
  submitInfo.pSignalSemaphores = semaphores;

  if (auto result = vkQueueSubmit(m_queue, 1, &submitInfo, commandBuffer.getFence()->getFence());
      result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}", "vkQueueSubmit",
              result, __FILE__, __LINE__);
  }
  commandBuffer.setState(VulkanCommandBuffer::State::Pending);
}

void VulkanQueue::waitIdle() { vkQueueWaitIdle(m_queue); }

}  // namespace GEngine