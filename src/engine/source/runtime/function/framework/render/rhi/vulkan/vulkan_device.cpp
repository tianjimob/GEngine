#include "vulkan_device.h"
#include "core/log/logger.h"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan_queue.h"


#include <memory>
#include <set>

namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VulkanDevice::VulkanDevice(VulkanRHI *rhi, VkPhysicalDevice gpu)
    : m_rhi(rhi), m_gpu(gpu) {

  vkGetPhysicalDeviceProperties(gpu, &m_gpuProp);

  LOG_INFO(LogVulkanRHI, "- DeviceName: {}", m_gpuProp.deviceName);
  LOG_INFO(LogVulkanRHI, "- API={}.{}.{} (0x{}) Driver=0x{} VendorId=0x{}",
           VK_VERSION_MAJOR(m_gpuProp.apiVersion),
           VK_VERSION_MINOR(m_gpuProp.apiVersion),
           VK_VERSION_PATCH(m_gpuProp.apiVersion), m_gpuProp.apiVersion,
           m_gpuProp.driverVersion, m_gpuProp.vendorID);
  LOG_INFO(LogVulkanRHI, "- DeviceID=0x{} Type={}", m_gpuProp.deviceID,
           string_VkPhysicalDeviceType(m_gpuProp.deviceType));
  LOG_INFO(LogVulkanRHI, "- Max Descriptor Sets Bound {}, Timestamps {}",
           m_gpuProp.limits.maxBoundDescriptorSets,
           m_gpuProp.limits.timestampComputeAndGraphics);
}

void VulkanDevice::init() {
  uint32_t queueCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &queueCount, nullptr);
  m_queueFamilyProps.resize(queueCount);
  vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &queueCount,
                                           m_queueFamilyProps.data());

  vkGetPhysicalDeviceFeatures(m_gpu, &m_physicalDeviceFeatures);

  LOG_INFO(LogVulkanRHI, "Device properties: Geometry {}",
           m_physicalDeviceFeatures.geometryShader);

  createDevice();
}

void VulkanDevice::createDevice() {
  // setup extensions and layers
  auto requiredExtensions = getRequiredExtensions();
  auto deviceLayers = setupLayers(requiredExtensions);
  std::vector<const char *> deviceExtensions;
  for (auto &extension : requiredExtensions) {
    if (extension.supported || extension.desired) {
      deviceExtensions.emplace_back(extension.name);
    }
  }

  // setup queue create info
  LOG_INFO(LogVulkanRHI, "Found {} Queue Families", m_queueFamilyProps.size());

  auto familyIndices = findQueueFamilies();
  std::vector<VkDeviceQueueCreateInfo>
      queueCreateInfos; // all queues that need to be created
  std::set<uint32_t> queueFamilies = {familyIndices.graphicsIndex.value(),
                                      familyIndices.computeIndex.value(),
                                      familyIndices.transferIndex.value()};
  queueCreateInfos.resize(queueFamilies.size());

  {
    float queuePriority = 1.0f;
    int i = 0;
    for (uint32_t queueFamily : queueFamilies) // for every queue family
    {
      // queue create info
      VkDeviceQueueCreateInfo &queueCreateInfo = queueCreateInfos[i];
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = m_queueFamilyProps[queueFamily].queueCount;
      queueCreateInfo.pQueuePriorities = &queuePriority;

      ++i;
    }
  }

  // device create info
  VkDeviceCreateInfo deviceCreateInfo;
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pNext = nullptr;
  deviceCreateInfo.pEnabledFeatures = &m_physicalDeviceFeatures;
  deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
  deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
  deviceCreateInfo.ppEnabledLayerNames = deviceLayers.data();
  deviceCreateInfo.enabledLayerCount = deviceLayers.size();
  deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
  deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();

  VkResult result =
      vkCreateDevice(m_gpu, &deviceCreateInfo, nullptr, &m_device);
  if (result == VK_ERROR_INITIALIZATION_FAILED) {
    LOG_FATAL(LogVulkanRHI, "Cannot create a Vulkan device. Try updating your "
                            "video driver to a more recent version.");

  } else if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}", "vkCreateDevice",
              result, __FILE__, __LINE__);
  }

  // initialize queue of this device

  m_graphicsQueue =
      std::make_shared<VulkanQueue>(*this, familyIndices.graphicsIndex.value());
  m_computeQueue =
      std::make_shared<VulkanQueue>(*this, familyIndices.computeIndex.value());
  m_transferQueue =
      std::make_shared<VulkanQueue>(*this, familyIndices.transferIndex.value());

  // more efficient pointer
  _vkResetCommandPool = (PFN_vkResetCommandPool)vkGetDeviceProcAddr(
      m_device, "vkResetCommandPool");
  _vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(
      m_device, "vkBeginCommandBuffer");
  _vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(
      m_device, "vkEndCommandBuffer");
  _vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)vkGetDeviceProcAddr(
      m_device, "vkCmdBeginRenderPass");
  _vkCmdNextSubpass =
      (PFN_vkCmdNextSubpass)vkGetDeviceProcAddr(m_device, "vkCmdNextSubpass");
  _vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)vkGetDeviceProcAddr(
      m_device, "vkCmdEndRenderPass");
  _vkCmdBindPipeline =
      (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(m_device, "vkCmdBindPipeline");
  _vkCmdSetViewport =
      (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(m_device, "vkCmdSetViewport");
  _vkCmdSetScissor =
      (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(m_device, "vkCmdSetScissor");
  _vkWaitForFences =
      (PFN_vkWaitForFences)vkGetDeviceProcAddr(m_device, "vkWaitForFences");
  _vkResetFences =
      (PFN_vkResetFences)vkGetDeviceProcAddr(m_device, "vkResetFences");
  _vkCmdDrawIndexed =
      (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(m_device, "vkCmdDrawIndexed");
  _vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)vkGetDeviceProcAddr(
      m_device, "vkCmdBindVertexBuffers");
  _vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)vkGetDeviceProcAddr(
      m_device, "vkCmdBindIndexBuffer");
  _vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vkGetDeviceProcAddr(
      m_device, "vkCmdBindDescriptorSets");
  _vkCmdClearAttachments = (PFN_vkCmdClearAttachments)vkGetDeviceProcAddr(
      m_device, "vkCmdClearAttachments");

  LOG_INFO(LogVulkanRHI, "Using {} device layers {}", deviceLayers.size(),
           deviceLayers.size() > 0 ? ':' : '?');
  for (const char *layer : deviceLayers) {
    LOG_INFO(LogVulkanRHI, "* {}", layer);
  }

  LOG_INFO(LogVulkanRHI, "Using {} device extensions {}",
           deviceExtensions.size(), deviceExtensions.size() > 0 ? ':' : '?');
  for (const char *extension : deviceExtensions) {
    LOG_INFO(LogVulkanRHI, "* {}", extension);
  }

  setupFormats();
}

std::vector<VulkanExtension> VulkanDevice::getRequiredExtensions() {
  std::vector<VulkanExtension> ret;

  ret.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME, true);
  // ret.emplace_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
  // ret.emplace_back(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
  // ret.emplace_back(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_SPIRV_1_4_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME);
  // ret.emplace_back(VK_EXT_VALIDATION_CACHE_EXTENSION_NAME);
  // ret.emplace_back(VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);

  return ret;
}

std::vector<const char *>
VulkanDevice::setupLayers(std::vector<VulkanExtension> &requiredExtensions) {
  std::vector<const char *> ret;

  auto props = enumerateLayerProperties();

  LOG_INFO(LogVulkanRHI, "Found {} available device layers {}", props.size(),
           props.size() ? ":" : "!");
  for (const auto &prop : props) {
    LOG_INFO(LogVulkanRHI, "  * {}", prop.layerProperties.layerName);
  }

  auto findLayerContainExtension =
      [](const char *extensionName,
         const std::vector<LayerWithExtensions> &layerProperties) -> int32_t {
    for (int layerIndex = 0; layerIndex < layerProperties.size();
         ++layerIndex) {
      for (int i = 0;
           i < layerProperties[layerIndex].extensionProperties.size(); ++i) {
        if (strcmp(extensionName, layerProperties[layerIndex]
                                      .extensionProperties[i]
                                      .extensionName) == 0)
          return layerIndex;
      }
    }
    return -1;
  };

  auto supportExtension = [this, &requiredExtensions,
                           &findLayerContainExtension, &props,
                           &ret](const char *extensionName) {
    auto extIndex = -1;
    for (int i = 0; i < requiredExtensions.size(); ++i) {
      if (strcmp(extensionName, requiredExtensions[i].name) == 0) {
        extIndex = i;
        break;
      }
    }
    if (extIndex == -1)
      return false;

    if (!requiredExtensions[extIndex].supported) {
      auto layerIndex = findLayerContainExtension(extensionName, props);
      if (layerIndex != -1) {
        static VkLayerProperties prop;
        prop = props[layerIndex].layerProperties;
        addRequestedLayer(prop.layerName, props, requiredExtensions, ret);
      }
    }

    if (requiredExtensions[extIndex].supported) {
      return true;
    }
    return false;
  };

  for (auto &extension : requiredExtensions) {
    supportExtension(extension.name);
  }

  // deduplicate
  std::sort(ret.begin(), ret.end());
  auto last = std::unique(ret.begin(), ret.end());
  ret.erase(last, ret.end());

  return ret;
}

std::vector<LayerWithExtensions> VulkanDevice::enumerateLayerProperties() {
  std::vector<LayerWithExtensions> ret;
  std::vector<VkLayerProperties> layerProperties;

  uint32_t count = 0;
  {
    VkResult result = vkEnumerateDeviceLayerProperties(m_gpu, &count, nullptr);
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateDeviceLayerProperties", __FILE__, __LINE__);
    }
  }

  layerProperties.resize(count);
  ret.resize(count);
  {
    VkResult result =
        vkEnumerateDeviceLayerProperties(m_gpu, &count, layerProperties.data());
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateDeviceLayerProperties", __FILE__, __LINE__);
    }
  }

  for (int i = 0; i < count; ++i) {
    ret[i].layerProperties = layerProperties[i];
    ret[i].extensionProperties =
        getDriverSupportedInstanceExtensions(layerProperties[i].layerName);
  }

  return ret;
}

std::vector<VkExtensionProperties>
VulkanDevice::getDriverSupportedInstanceExtensions(const char *layerName) {
  std::vector<VkExtensionProperties> ret;

  uint32_t count = 0;
  {
    VkResult result =
        vkEnumerateDeviceExtensionProperties(m_gpu, layerName, &count, nullptr);
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateDeviceExtensionProperties", __FILE__, __LINE__);
    }
  }

  ret.resize(count);
  {
    VkResult result = vkEnumerateDeviceExtensionProperties(m_gpu, layerName,
                                                           &count, ret.data());
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateDeviceExtensionProperties", __FILE__, __LINE__);
    }
  }

  return ret;
}

bool VulkanDevice::addRequestedLayer(
    const char *layerName,
    const std::vector<LayerWithExtensions> &layerProperties,
    std::vector<VulkanExtension> &engineExtensions,
    std::vector<const char *> &layers) {
  int32_t index = -1;
  for (int i = 0; i < layerProperties.size(); ++i) {
    if (strcmp(layerName, layerProperties[i].layerProperties.layerName) == 0) {
      index = i;
      break;
    }
  }
  if (index == -1)
    return false;

  layers.emplace_back(layerName);

  for (const auto &extensionProperties :
       layerProperties[index].extensionProperties) {
    for (auto &extension : engineExtensions) {
      if (strcmp(extension.name, extensionProperties.extensionName) == 0)
        extension.supported = true;
    }
  }

  return true;
}

VulkanDevice::QueueFamilyIndices VulkanDevice::findQueueFamilies() {
  QueueFamilyIndices indices;

  int i = 0;
  for (const auto &prop : m_queueFamilyProps) {
    if (prop.queueFlags &
        VK_QUEUE_GRAPHICS_BIT) // if support graphics command queue
    {
      if (!indices.graphicsIndex.has_value()) {
        indices.graphicsIndex = i;
      } else if (prop.queueCount >
                 m_queueFamilyProps[indices.graphicsIndex.value()].queueCount)
        indices.graphicsIndex = i;
    }

    if (prop.queueFlags &
        VK_QUEUE_COMPUTE_BIT) // if support compute command queue
    {
      if (!indices.computeIndex.has_value() && indices.graphicsIndex != i) {
        indices.computeIndex = i;
      }
    }

    if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT) {
      if (!indices.computeIndex.has_value() &&
          !(prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
          !(prop.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
        indices.transferIndex = i;
      }
    }

    i++;
  }

  if (!indices.computeIndex.has_value())
    indices.computeIndex = indices.graphicsIndex;
  else
    supportAsyncCompute = true;

  if (!indices.transferIndex.has_value())
    indices.transferIndex = indices.computeIndex;

  assert(indices.isComplete() && "Cannot find graphics queue");

  return indices;
}

void VulkanDevice::setupFormats() {
  // todo: find formats physical device supported
}

} // namespace GEngine