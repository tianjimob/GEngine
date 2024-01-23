#pragma once

#include <vector>

#include "vulkan/vulkan.h"

namespace GEngine {

struct VulkanExtension {
  VulkanExtension() = default;

  VulkanExtension(const char *name) : name(name) {}

  VulkanExtension(const char *name, bool desired)
      : name(name), desired(desired) {}

  const char *name;
  bool supported{false};
  bool desired{false};
};

struct LayerWithExtensions {
  VkLayerProperties layerProperties;
  std::vector<VkExtensionProperties> extensionProperties;
};

}  // namespace GEngine