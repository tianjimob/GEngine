#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "function/framework/render/rhi/vulkan/vulkan_context.h"
#include "function/framework/render/rhi/vulkan/vulkan_pipeline.h"
#include "function/framework/render/rhi/vulkan/vulkan_pipeline_state.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_com.h"

namespace GEngine {

class VulkanRHI;
class VulkanQueue;

class VulkanDevice {
 public:
  VulkanDevice(VulkanRHI *rhi, VkPhysicalDevice gpu);

  void init();

  const VkPhysicalDeviceProperties &getGpuProperties() { return m_gpuProp; }

  VkDevice getDevice() { return m_device; }

  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags propertiesFlag);

  VkShaderModule createShaderModule(const std::vector<uint8_t> &shaderCode);

  VulkanPipelineStateCacheManager &getPipelineManager() {
    return m_pipelineStateCacheManager;
  }

  VulkanDescriptorPoolManager &getDescriptorPoolManager() {
    return m_descriptorPoolManager;
  }

  std::shared_ptr<VulkanRHICommandContext> &getGraphicsContext() {
    return m_graphicsContext;
  }

  std::shared_ptr<VulkanRHICommandContext> &getComputeContext() {
    return m_computeContext;
  }

 private:
  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsIndex;
    std::optional<uint32_t> computeIndex;
    std::optional<uint32_t> transferIndex;

    bool isComplete() {
      return graphicsIndex.has_value() && computeIndex.has_value() &&
             transferIndex.has_value();
    }
  };

  void createDevice();

  std::vector<VulkanExtension> getRequiredExtensions();

  std::vector<const char *> setupLayers(
      std::vector<VulkanExtension> &requiredExtensions);

  std::vector<LayerWithExtensions> enumerateLayerProperties();

  std::vector<VkExtensionProperties> getDriverSupportedInstanceExtensions(
      const char *layerName);

  bool addRequestedLayer(
      const char *layerName,
      const std::vector<LayerWithExtensions> &layerProperties,
      std::vector<VulkanExtension> &engineExtensions,
      std::vector<const char *> &layers);

  QueueFamilyIndices findQueueFamilies();

  void setupFormats();

 private:
  VulkanRHI *m_rhi;  // pointer to VulkanRHI this device belog to

  VkPhysicalDevice m_gpu;
  VkPhysicalDeviceProperties m_gpuProp;
  VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;

  std::vector<VkQueueFamilyProperties> m_queueFamilyProps;
  VkPhysicalDeviceFeatures m_physicalDeviceFeatures;

  VkDevice m_device;
  std::shared_ptr<VulkanQueue> m_graphicsQueue;
  std::shared_ptr<VulkanQueue> m_computeQueue;
  std::shared_ptr<VulkanQueue> m_transferQueue;

  std::shared_ptr<VulkanRHICommandContext> m_graphicsContext;
  std::shared_ptr<VulkanRHICommandContext> m_computeContext;

  VulkanPipelineStateCacheManager m_pipelineStateCacheManager;
  VulkanDescriptorPoolManager m_descriptorPoolManager;

  // function pointers for this device
  PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
  PFN_vkCmdEndDebugUtilsLabelEXT _vkCmdEndDebugUtilsLabelEXT;
  PFN_vkWaitForFences _vkWaitForFences;
  PFN_vkResetFences _vkResetFences;
  PFN_vkResetCommandPool _vkResetCommandPool;
  PFN_vkBeginCommandBuffer _vkBeginCommandBuffer;
  PFN_vkEndCommandBuffer _vkEndCommandBuffer;
  PFN_vkCmdBeginRenderPass _vkCmdBeginRenderPass;
  PFN_vkCmdNextSubpass _vkCmdNextSubpass;
  PFN_vkCmdEndRenderPass _vkCmdEndRenderPass;
  PFN_vkCmdBindPipeline _vkCmdBindPipeline;
  PFN_vkCmdSetViewport _vkCmdSetViewport;
  PFN_vkCmdSetScissor _vkCmdSetScissor;
  PFN_vkCmdBindVertexBuffers _vkCmdBindVertexBuffers;
  PFN_vkCmdBindIndexBuffer _vkCmdBindIndexBuffer;
  PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
  PFN_vkCmdDrawIndexed _vkCmdDrawIndexed;
  PFN_vkCmdClearAttachments _vkCmdClearAttachments;

  bool supportAsyncCompute{false};
};

}  // namespace GEngine