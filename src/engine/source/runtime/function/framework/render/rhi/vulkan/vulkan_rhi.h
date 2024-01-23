#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "function/framework/render/rhi/rhi.h"
#include "vulkan_com.h"
#include "vulkan_device.h"


namespace GEngine {

class VulkanRHI : public RHI {
 public:
  VulkanRHI();

  virtual void init() override;
  virtual std::string_view getName() override { return "Vulkan"; }

  virtual RHIGraphicsPipelineStateRef createGraphicsPipelineState(
      const RHIGraphicsPipelineStateCreateInfo &createInfo) override {
    return {};
  }

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

  void setupDebugLayerCallback();

  bool isDeviceSuitable(const VulkanDevice &physicalDevice);

 private:
  VkInstance m_instance;

  std::vector<std::shared_ptr<VulkanDevice>> m_devices;
  std::weak_ptr<VulkanDevice>
      m_device;  // pointer to one of m_devices (now best fit)

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