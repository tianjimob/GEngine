#include "vulkan_queue.h"
#include "vulkan_device.h"

namespace GEngine {

VulkanQueue::VulkanQueue(VulkanDevice &device, uint32_t queueFamilyIndex)
    : m_device(device), m_queueIndex(0) {
  vkGetDeviceQueue(device.getDeviceHanle(), queueFamilyIndex, m_queueIndex,
                   &m_queue);
}

} // namespace GEngine