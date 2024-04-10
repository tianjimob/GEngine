#include "vulkan_sync.h"

#include "core/log/logger.h"
#include "vulkan_device.h"
#include <cassert>


namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VulkanSemaphore::VulkanSemaphore(VulkanDevice &device) : m_device(device) {
  VkSemaphoreCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  createInfo.pNext = nullptr;
  auto result = vkCreateSemaphore(m_device.getDevice(), &createInfo, nullptr,
                                  &m_semaphore);
  if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkCreateSemaphore", result, __FILE__, __LINE__);
  }
}

VulkanSemaphore::~VulkanSemaphore()
{
  vkDestroySemaphore(m_device.getDevice(), m_semaphore, nullptr);
}

VulkanFence::VulkanFence(VulkanDevice &device, bool createSignaled):m_device(device), m_signaled(createSignaled)
{
  VkFenceCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = createSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
  if (auto result =
          vkCreateFence(device.getDevice(), &createInfo, nullptr, &m_fence); result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkCreateFence", result, __FILE__, __LINE__);
  }
}

VulkanFence::~VulkanFence()
{
  vkDestroyFence(m_device.getDevice(), m_fence, nullptr);
}

bool VulkanFence::isSingaled()
{
  if (m_signaled)
    return true;

  auto result = vkGetFenceStatus(m_device.getDevice(), m_fence);
  switch (result) {
  case VK_SUCCESS:
    m_signaled = true;
    return true;
  case VK_NOT_READY:
    break;
  default:
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkGetFenceStatus", result, __FILE__, __LINE__);
    break;
  }
  return false;
}

bool VulkanFence::wait(uint64_t ns)
{
  assert(m_signaled == false);
  switch (auto result = vkWaitForFences(m_device.getDevice(), 1, &m_fence, true, ns)) {
  case VK_SUCCESS:
    m_signaled = true;
    return true;
  case VK_TIMEOUT:
    break;
  default:
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkWaitForFences", result, __FILE__, __LINE__);
  }
  return false;
}

void VulkanFence::reset()
{
  if (m_signaled) {
    if (auto result = vkResetFences(m_device.getDevice(), 1, &m_fence);
        result != VK_SUCCESS) {
      LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkResetFences", result, __FILE__, __LINE__);
    }
    m_signaled = false;
  }
}



}  // namespace GEngine