#pragma once

#include "vulkan/vulkan.h"

namespace GEngine {

class VulkanDevice;
class VulkanCommandBuffer;

class VulkanQueue {
public:
  VulkanQueue(VulkanDevice &device, uint32_t queueFamilyIndex);
  uint32_t getFamilyIndex() { return m_queueFamilyIndex; }
  void submit(VulkanCommandBuffer &commandBuffer, uint32_t nSemaphores,
              VkSemaphore *semaphores);
  void waitIdle();

private:
  VulkanDevice &m_device;
  uint32_t m_queueFamilyIndex;
  uint32_t m_queueIndex;
  VkQueue m_queue;
};

}