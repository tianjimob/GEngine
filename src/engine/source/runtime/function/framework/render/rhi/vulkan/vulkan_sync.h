#pragma once

#include "vulkan/vulkan.h"

namespace GEngine {

class VulkanDevice;

class VulkanSemaphore {
public:
  VulkanSemaphore(VulkanDevice &device);
  ~VulkanSemaphore();

  VkSemaphore getSemaphore() { return m_semaphore; }
private:
  VulkanDevice &m_device;
  VkSemaphore m_semaphore;
};

class VulkanFence {
public:
  VulkanFence(VulkanDevice &device, bool createSignaled = false);
  ~VulkanFence();

  VkFence getFence() { return m_fence; }
  bool isSingaled();

  bool wait(uint64_t ns);
  void reset();

private:
  VulkanDevice &m_device;
  bool m_signaled;
  VkFence m_fence;
  
};

class VulkanFenceManager {};

}