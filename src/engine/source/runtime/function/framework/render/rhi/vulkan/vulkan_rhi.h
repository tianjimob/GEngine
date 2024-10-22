#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "function/framework/render/rhi/rhi.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_com.h"
#include "vulkan_device.h"


namespace GEngine {

class VulkanRHI : public RHI {
 public:
  VulkanRHI();

  VkInstance getInstance() { return m_instance; }
  std::shared_ptr<VulkanDevice> getActiveDevice() { return m_device.lock(); }

  virtual void init() override;
  virtual std::string_view getName() override { return "Vulkan"; }

  virtual std::shared_ptr<RHIGraphicsPipelineState> createGraphicsPipelineState(
      const RHIGraphicsPipelineStateInitiazlier &createInfo) override;
  
  virtual std::shared_ptr<RHIComputePipelineState> createComputePipelineState(std::shared_ptr<RHIComputeShader>& computeShader) override;

  virtual std::shared_ptr<RHIUniformBuffer> createUniformBuffer(uint32_t size) override;

  virtual std::shared_ptr<RHIBuffer>
  createBuffer(uint32_t size, RHIBufferUsageFlags usage,
               RHIMemoryPropertyFlags property) override;

  virtual std::shared_ptr<RHIBuffer> createVertexBuffer(const void *data,
                                                        uint32_t size) override;

  virtual std::shared_ptr<RHIBuffer> createIndexBuffer(const void *data, uint32_t size) override;

  virtual std::shared_ptr<RHIComputeShader>
  createComputeShader(const std::vector<uint8_t> &shaderCode) override;

  virtual std::shared_ptr<RHIRasterizerState> createRasterizerState(
      const RHIRasterizerStateInitializer &initializer) override;

  virtual std::shared_ptr<RHIDepthStencilState> createDepthStencilweState(
      const RHIDepthStencilStateInitializer &initializer) override;

  virtual std::shared_ptr<RHIBlendState>
  createBlendState(const RHIBlendStateInitializer &initializer) override;

  virtual void mapBuffer(void *data, std::shared_ptr<RHIBuffer> &buffer) override;

  virtual void unmapBuffer(std::shared_ptr<RHIBuffer> &buffer) override;

  virtual std::shared_ptr<RHICommandContext> getComputeContext() override;

  virtual std::shared_ptr<RHICommandContext> getGraphicsContext() override;

  virtual std::shared_ptr<RHIViewport> createViewport() override;

 private:
  void createInstance();

  void selectDevice();

  std::vector<VulkanExtension> getRequiredExtensions();

  std::vector<const char *> setupLayers(
      std::vector<VulkanExtension> &engineExtensions);

  std::vector<LayerWithExtensions> enumerateLayerProperties();

  std::vector<VkExtensionProperties> getDriverSupportedInstanceExtensions(
      const char *layerName);

  bool addRequestedLayer(
      const char *layerName,
      const std::vector<LayerWithExtensions> &layerProperties,
      std::vector<VulkanExtension> &engineExtensions,
      std::vector<const char *> &layers);

  void populateDebugUtilsMessengerCreateInfoEXT(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  
  void setupDebugLayerCallback();

  bool isDeviceSuitable(const VulkanDevice &physicalDevice);

  void createBuffer(uint32_t size, RHIBufferUsageFlags usage,
               RHIMemoryPropertyFlags property, VkBuffer& buffer, VkDeviceMemory& memory);

 private:
  VkInstance m_instance;
  VkDebugUtilsMessengerEXT m_debugMessenger;

  std::vector<std::shared_ptr<VulkanDevice>> m_devices;
  std::weak_ptr<VulkanDevice>
      m_device; // pointer to one of m_devices (now best fit)

 private:
  enum class DebugLayerExtension {
    None,
    Gfxreconstruct,
    VkTrace,
    DebugUtils,
    DebugReport
  };

  std::vector<const char *> m_instanceExtensions;
  std::vector<const char *> m_instanceLayers;
  DebugLayerExtension activeDebugLayerExtension{DebugLayerExtension::None};
};

}  // namespace GEngine