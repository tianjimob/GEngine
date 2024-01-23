#pragma once

#include "vulkan/vulkan.h"

namespace GEngine {

class VulkanDevice;

class VulkanQueue {
public:
  VulkanQueue(VulkanDevice& device, uint32_t queueFamilyIndex);

private:
  VulkanDevice &m_device;
  uint32_t m_queueIndex;
  VkQueue m_queue;
};

}