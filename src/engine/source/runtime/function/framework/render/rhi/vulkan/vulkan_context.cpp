#include "vulkan_context.h"
#include <memory>
#include <vector>
#include "vulkan_command_buffer.h"
namespace GEngine {

VulkanRHICommandContext::VulkanRHICommandContext(VulkanDynamicRHI *rhi,
                                                 VulkanDevice *device,
                                                 VulkanQueue *queue,
                                                 bool isImmediate):m_rhi(rhi),m_device(device),m_queue(queue), m_isImmediate(isImmediate) {
  m_commandBufferManager =
      std::make_shared<VulkanCommandBufferManager>(m_device, this);
  m_commandBufferManager->init();
  if (m_isImmediate) {
    m_commandBufferManager->submitActiveCommandBuffer(
        std::vector<std::shared_ptr<VulkanSemaphore>>{});
    m_commandBufferManager->prepareForNewActiveCommandBuffer();
  }
                          }

}