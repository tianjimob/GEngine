#include "vulkan_rhi_resource.h"

#include "function/framework/render/rhi/vulkan/vulkan_device.h"
#include "function/framework/render/rhi/vulkan/vulkan_pipeline.h"
#include "vulkan/vulkan_core.h"


namespace GEngine {
void VulkanMemory::free() {
  if (m_vulkanDevice) {
    vkFreeMemory(m_vulkanDevice->getDevice(), m_memory, nullptr);
  }
}

VulkanRHIBuffer::~VulkanRHIBuffer() {
  m_vulkanMemory.free();
  if (m_vulkanDevice != nullptr)
    vkDestroyBuffer(m_vulkanDevice->getDevice(), m_buffer, nullptr);
}

VulkanRHIUniformBuffer::~VulkanRHIUniformBuffer() {
  vkUnmapMemory(m_vulkanDevice->getDevice(), m_vulkanMemory.getMemory());
  m_vulkanMemory.free();
  if (m_vulkanDevice != nullptr)
    vkDestroyBuffer(m_vulkanDevice->getDevice(), m_buffer, nullptr);
}

VulkanRHIComputeShader::~VulkanRHIComputeShader() {
  vkDestroyShaderModule(m_device->getDevice(), m_shaderModule, nullptr);
}

VkPipelineLayout VulkanRHIComputePipelineState::getPipelineLayout() {
  return m_vulkanLayout->getPipelineLayout();
}

VulkanRHIVertexShader::~VulkanRHIVertexShader() {
  vkDestroyShaderModule(m_device->getDevice(), m_shaderModule, nullptr);
}

VulkanRHIPixelShader::~VulkanRHIPixelShader() {
  vkDestroyShaderModule(m_device->getDevice(), m_shaderModule, nullptr);
}
}  // namespace GEngine