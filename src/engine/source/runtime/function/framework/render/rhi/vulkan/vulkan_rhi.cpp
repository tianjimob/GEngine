#include "vulkan_rhi.h"

#include "SDL2/SDL_vulkan.h"
#include "core/log/logger.h"
#include "function/framework/engine/engine.h"
#include "function/framework/engine/game_engine/game_engine.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/rhi_type.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "platform/vulkan/vulkan_vendor.h"
#include "vulkan/vulkan_core.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <malloc.h>
#include <memory>
#include <vector>

namespace GEngine {

#define GENGINE_VULKAN_API_VERSION                                             \
  ((((uint32_t)(0)) << 29) | (((uint32_t)(1)) << 22) |                         \
   (((uint32_t)(2)) << 12) | ((uint32_t)(0)))

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VulkanRHI::VulkanRHI() {
  LOG_INFO(LogVulkanRHI, "Built with Vulkan header version {}.{}.{}",
           VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE),
           VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
           VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE));

  createInstance();
  selectDevice();
}

void VulkanRHI::init() {
  if (auto device = m_device.lock()) {
    device->init();
  }
}

std::shared_ptr<RHIComputePipelineState>
VulkanRHI::createComputePipelineState(std::shared_ptr<RHIComputeShader>& computeShader) {
  if (auto it = m_device.lock()) {
    return it->getPipelineManager().getOrCreateComputePipeline(
        std::static_pointer_cast<VulkanRHIComputeShader>(computeShader));
  }
  return nullptr;
}

std::shared_ptr<RHIUniformBuffer> VulkanRHI::createUniformBuffer(uint32_t size) {
  VkBuffer buffer;
  VkDeviceMemory memory;
  createBuffer(size, RHIBufferUsageFlags::UniformBuffer,
               RHIMemoryPropertyFlags::HostVisibleAndCoherent, buffer, memory);
  VulkanMemory vulkanMemory{m_device.lock().get(), memory};
  void *mapped;
  auto device = m_device.lock();
  vkMapMemory(device->getDevice(), memory, 0, size, 0, &mapped);
  return std::make_shared<VulkanRHIUniformBuffer>(m_device.lock().get(), buffer,
                                                  vulkanMemory, mapped);
}

std::shared_ptr<RHIBuffer> VulkanRHI::createBuffer(uint32_t size,
                                                   RHIBufferUsageFlags usage, RHIMemoryPropertyFlags property) {

  VkBuffer buffer;
  VkDeviceMemory memory;
  createBuffer(size,usage,property, buffer, memory);                                                  
  VulkanMemory vulkanMemory{m_device.lock().get(), memory};
  std::shared_ptr<RHIBuffer> ret = std::make_shared<VulkanRHIBuffer>(
      size,  m_device.lock().get(), buffer,vulkanMemory);
  return ret;
}

std::shared_ptr<RHIBuffer> VulkanRHI::createVertexBuffer(const void *data,
                                                         uint32_t size) {
  std::shared_ptr<RHIBuffer> stagingBuffer =
      createBuffer(size, RHIBufferUsageFlags::TransferSrc,
                   RHIMemoryPropertyFlags::HostVisibleAndCoherent);
  VkDeviceMemory stagingMemory =
      std::static_pointer_cast<VulkanRHIBuffer>(stagingBuffer)->getMemory();

  std::shared_ptr<VulkanDevice> device = m_device.lock();

  void *dstData;
  vkMapMemory(device->getDevice(), stagingMemory, 0, size, 0, &dstData);
  memcpy(dstData, data, size);
  vkUnmapMemory(device->getDevice(), stagingMemory);

  std::shared_ptr<RHIBuffer> vertexBuffer =
      createBuffer(size, RHIBufferUsageFlags::TransferSrcAndVertexBuffer,
                   RHIMemoryPropertyFlags::DeviceLocal);

  device->getGraphicsContext()->RHICopyBuffer(stagingBuffer, vertexBuffer);

  return vertexBuffer;
}

std::shared_ptr<RHIComputeShader>
VulkanRHI::createComputeShader(const std::vector<uint8_t> &shaderCode) {
  if (auto device = m_device.lock()) {
    VkShaderModule shaderModule = device->createShaderModule(shaderCode);
    if (shaderModule != VK_NULL_HANDLE) {
      std::shared_ptr<RHIComputeShader> shader =
          std::make_shared<VulkanRHIComputeShader>(device.get(), shaderModule);
      return shader;
    } else {
      LOG_ERROR(LogVulkanRHI, "createComputerShader failed!");
      return nullptr;
    }
  }
  return nullptr;
}

void VulkanRHI::createInstance() {
  VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "DefaultApp";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "GEngine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = GENGINE_VULKAN_API_VERSION;
  appInfo.pNext = nullptr;

  VkInstanceCreateInfo instanceInfo;
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.pNext = nullptr;

  auto engineExtensions = getRequiredExtensions();
  m_instanceLayers = setupLayers(engineExtensions);
  for (auto &extension : engineExtensions) {
    if (extension.supported || extension.desired) {
      m_instanceExtensions.emplace_back(extension.name);
    }
  }
  instanceInfo.enabledExtensionCount = m_instanceExtensions.size();
  instanceInfo.ppEnabledExtensionNames =
      m_instanceExtensions.size() ? m_instanceExtensions.data() : nullptr;
  instanceInfo.enabledLayerCount = m_instanceLayers.size();
  instanceInfo.ppEnabledLayerNames =
      m_instanceLayers.size() ? m_instanceLayers.data() : nullptr;

  VkResult result = vkCreateInstance(&instanceInfo, nullptr, &m_instance);
  if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
    LOG_FATAL(LogVulkanRHI, "Cannot find a compatible Vulkan driver (ICD).");
    return;
  } else if (result == VK_ERROR_EXTENSION_NOT_PRESENT) {
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> props;
    props.resize(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

    for (auto *extension : m_instanceExtensions) {
      bool found = false;

      for (auto &prop : props) {
        if (strcmp(extension, prop.extensionName) == 0) {
          found = true;
          break;
        }
      }

      if (!found) {
        LOG_ERROR(LogVulkanRHI, "Missing required Vulkan extension: {}",
                  *extension);
      }
    }

    LOG_FATAL(LogVulkanRHI,
              "Vulkan driver doesn't contain specified extensions! make sure "
              "your layers path is set appropriately.");
  } else if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "Vulkan failed to create instance. Do you have a "
                            "compatible Vulkan driver (ICD) installed?");
  }

#ifdef VULKAN_DEBUG_ENABLE
  setupDebugLayerCallback();
#endif
}

void VulkanRHI::selectDevice() {
  uint32_t gpuCount = 0;
  {
    VkResult result =
        vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_FATAL(LogVulkanRHI,
                "Cannot find a compatible Vulkan device or driver. Try "
                "updating your video driver to a more recent version and make "
                "sure your video card supports Vulkan.");
    }
  }
  assert(gpuCount >= 1 &&
         "No GPU(s)/Driver(s) that support Vulkan were found! Make sure your "
         "drivers are up to date and that you are not pending a reboot.");

  std::vector<VkPhysicalDevice> physicalDevices;
  physicalDevices.resize(gpuCount);
  {
    VkResult result = vkEnumeratePhysicalDevices(m_instance, &gpuCount,
                                                 physicalDevices.data());
    if (result != VK_SUCCESS) {
      LOG_FATAL(LogVulkanRHI, "{} failed. VkResult={} at {}:{}",
                "vkEnumeratePhysicalDevices", result, __FILE__, __LINE__);
    }
  }

  struct RankDevice {
    int score;
    int indexInDevices;
  };
  std::vector<RankDevice> rankedPhysicalDevices;
  int i = 0;
  for (VkPhysicalDevice device : physicalDevices) {
    auto &vulkanDevice = m_devices.emplace_back(std::make_shared<VulkanDevice>(this, device));

    const auto &prop = vulkanDevice->getGpuProperties();

    int score = 0;

    if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      score += 1000;
    else if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
      score += 100;

    if (prop.apiVersion >= VK_API_VERSION_1_3)
      score += 100;
    else if (prop.apiVersion >= VK_API_VERSION_1_2)
      score += 50;
    else if (prop.apiVersion >= VK_API_VERSION_1_1)
      score += 20;

    switch (prop.vendorID) {
    case VULKAN_VENDOR_NVIDIA:
      score += 100;
      break;
    case VULKAN_VENDOR_AMD:
      score += 90;
      break;
    case VULKAN_VENDOR_INTEL:
      score += 70;
      break;
    case VULKAN_VENDOR_APPLE:
      score += 70;
      break;
    case VULKAN_VENDOR_ARM:
      score += 40;
      break;
    case VULKAN_VENDOR_QUALCOMM:
      score += 40;
      break;
    default:
      break;
    }

    rankedPhysicalDevices.push_back({score, i});
    LOG_INFO(LogVulkanRHI, "[{}] {} points", prop.deviceName, score);
  }

  std::sort(rankedPhysicalDevices.begin(), rankedPhysicalDevices.end(),
            [](const RankDevice &a, const RankDevice &b) {
              return a.score > b.score;
            });

  for (const auto &device : rankedPhysicalDevices) {
    if (isDeviceSuitable(*m_devices[device.indexInDevices])) {
      m_device = m_devices[device.indexInDevices];
      break;
    }
  }

  if (!m_device.lock()) {
    LOG_FATAL(LogVulkanRHI, "Failed to find suitable physical device");
  }
}

std::vector<VulkanExtension> VulkanRHI::getRequiredExtensions() {
  std::vector<VulkanExtension> engineExtensions;

  SDL_Window *sdlWindow = nullptr;
  if (GlobalEngine) {
    GameEngine *gameEngine = (GameEngine *)GlobalEngine;
    Window *window = (Window *)gameEngine->getWindow();
    if (window) {
      sdlWindow = (SDL_Window *)window->getNativeHandle();
    }
  }

  if (sdlWindow) {
    unsigned int numExtensions = 0;
    std::vector<const char *> extensions;
    SDL_Vulkan_GetInstanceExtensions(sdlWindow, &numExtensions, NULL);
    extensions.resize(numExtensions);
    SDL_Vulkan_GetInstanceExtensions(sdlWindow, &numExtensions,
                                     extensions.data());
    engineExtensions.resize(numExtensions);
    for (int i = 0; i < numExtensions; ++i) {
      engineExtensions[i].name = extensions[i];
      engineExtensions[i].desired = true;
    }
  }

  // Generic extensions
  engineExtensions.emplace_back(
      VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

  // Debug extensions
  engineExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  engineExtensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

  // Validation extensions
  engineExtensions.emplace_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);

  return engineExtensions;
}

std::vector<const char *>
VulkanRHI::setupLayers(std::vector<VulkanExtension> &engineExtensions) {
  std::vector<const char *> ret;

  auto props = enumerateLayerProperties();

  LOG_INFO(LogVulkanRHI, "Found {} available instance layers {}", props.size(),
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

  auto supportExtension = [this, &engineExtensions, &findLayerContainExtension,
                           &props, &ret](const char *extensionName) {
    auto extIndex = -1;
    for (int i = 0; i < engineExtensions.size(); ++i) {
      if (strcmp(extensionName, engineExtensions[i].name) == 0) {
        extIndex = i;
        break;
      }
    }
    if (extIndex == -1)
      return false;

    if (!engineExtensions[extIndex].supported) {
      auto layerIndex = findLayerContainExtension(extensionName, props);
      if (layerIndex != -1) {
        static VkLayerProperties prop;
        prop = props[layerIndex].layerProperties;
        addRequestedLayer(prop.layerName, props, engineExtensions, ret);
      }
    }

    if (engineExtensions[extIndex].supported) {
      return true;
    }
    return false;
  };

  // support desired extension
  {
    for (auto &extension : engineExtensions) {
      if (extension.desired) {
        supportExtension(extension.name);
      }
    }
  }

  // add debug layer
  {
    if (!addRequestedLayer("VK_LAYER_LUNARG_gfxreconstruct", props,
                           engineExtensions, ret)) {
      if (addRequestedLayer("VK_LAYER_LUNARG_vktrace", props, engineExtensions,
                            ret))
        activeDebugLayerExtension = DebugLayerExtension::VkTrace;
    } else
      activeDebugLayerExtension = DebugLayerExtension::Gfxreconstruct;

#ifdef VULKAN_DEBUG_ENABLE
    if (!addRequestedLayer("VK_LAYER_LUNARG_api_dump", props, engineExtensions,
                           ret)) {
      LOG_WARN(LogVulkanRHI,
               "Unable to find Vulkan instance layer VK_LAYER_LUNARG_api_dump");
    }

    if (activeDebugLayerExtension == DebugLayerExtension::None) {
      if (!supportExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
        if (supportExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
          activeDebugLayerExtension = DebugLayerExtension::DebugReport;
        }
      } else
        activeDebugLayerExtension = DebugLayerExtension::DebugUtils;
    }
#endif
  }

#if defined(VULKAN_DEBUG_ENABLE) || defined(VULKAN_VALIDATION_ENABLE)
  // add validation layer
  if (activeDebugLayerExtension == DebugLayerExtension::None) {
    if (!addRequestedLayer("VK_LAYER_KHRONOS_validation", props,
                           engineExtensions, ret)) {
      LOG_WARN(LogVulkanRHI, "Unable to find Vulkan instance validation layer "
                             "VK_LAYER_KHRONOS_validation");
      if (!addRequestedLayer("VK_LAYER_LUNARG_standard_validation", props,
                             engineExtensions, ret)) {
        LOG_WARN(LogVulkanRHI, "Unable to find Vulkan instance validation "
                               "layer VK_LAYER_LUNARG_standard_validation");
      }
    }

    supportExtension(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
  }
#endif

  // deduplicate
  std::sort(ret.begin(), ret.end());
  auto last = std::unique(ret.begin(), ret.end());
  ret.erase(last, ret.end());

  return ret;
}

bool VulkanRHI::addRequestedLayer(
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

// debug callback
static VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *userData) {

  const char *serverityStr = nullptr;
  if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    serverityStr = "Error";
  } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    serverityStr = "Warn";
  } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
    serverityStr = "Info";
  } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    serverityStr = "Verbose";
  }

  const char *typeStr = nullptr;
  if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
    typeStr = "General";
  } else if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
    typeStr = "Validation";
  } else if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
    typeStr = "Performance";
  } else if (type &
             VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT) {
    typeStr = "DeviceAddressBinding";
  }

  LOG_WARN(LogVulkanRHI, "*** [{}:{}({})] {}", serverityStr, typeStr,
           pCallbackData->messageIdNumber, pCallbackData->pMessage);

  return VK_FALSE;
}

static VkBool32 VKAPI_PTR debugReportCallback(
    VkDebugReportFlagsEXT msgFlags, VkDebugReportObjectTypeEXT objType,
    uint64_t srcObject, size_t location, int32_t msgCode,
    const char *layerPrefix, const char *msg, void *userData) {
  const char *msgPrefix = "UNKNOWN";
  if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    // Ignore some errors we might not fix...
    if (!strcmp(layerPrefix, "Validation")) {
      if (msgCode == 0x4c00264) {
        // Unable to allocate 1 descriptorSets from pool 0x8cb8. This pool only
        // has N descriptorSets remaining. The spec valid usage text states
        // 'descriptorSetCount must not be greater than the number of sets that
        // are currently available for allocation in descriptorPool'
        // (https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VUID-VkDescriptorSetAllocateInfo-descriptorSetCount-00306)
        return VK_FALSE;
      } else if (msgCode == 0x4c00266) {
        // Unable to allocate 1 descriptors of type
        // VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER from pool 0x89f4. This pool only
        // has 0 descriptors of this type remaining.The spec valid usage text
        // states 'descriptorPool must have enough free descriptor capacity
        // remaining to allocate the descriptor sets of the specified layouts'
        // (https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VUID-VkDescriptorSetAllocateInfo-descriptorPool-00307)
        return VK_FALSE;
      }
    }
    if (!strcmp(layerPrefix, "SC")) {
      if (msgCode == 3) {
        // Attachment N not written by fragment shader
        return VK_FALSE;
      }
    }
    if (!strcmp(layerPrefix, "DS")) {
      if (msgCode == 6) {
        auto *Found = strstr(msg, " array layer ");
        if (Found && Found[13] >= '1' && Found[13] <= '9') {
          // #todo-rco: Remove me?
          //  Potential bug in the validation layers for slice > 1 on 3d
          //  textures
          return VK_FALSE;
        }
      } else if (msgCode == 15) {
        // Cannot get query results on queryPool 0x327 with index 193 as data
        // has not been collected for this index.
        // return VK_FALSE;
      }
    }

    msgPrefix = "ERROR";
  } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
    msgPrefix = "WARN";

    // Ignore some warnings we might not fix...
    // Ignore some errors we might not fix...
    if (!strcmp(layerPrefix, "Validation")) {
      if (msgCode == 2) {
        // fragment shader writes to output location 0 with no matching
        // attachment
        return VK_FALSE;
      }
    }

    if (!strcmp(layerPrefix, "SC")) {
      if (msgCode == 2) {
        // fragment shader writes to output location 0 with no matching
        // attachment
        return VK_FALSE;
      }
    }
  } else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    msgPrefix = "PERF";
    // Ignore some errors we might not fix...
    if (!strcmp(layerPrefix, "SC")) {
      if (msgCode == 2) {
        // vertex shader outputs unused interpolator
        return VK_FALSE;
      }
    } else if (!strcmp(layerPrefix, "DS")) {
      if (msgCode == 15) {
        // DescriptorSet previously bound is incompatible with set newly bound
        // as set #0 so set #1 and any subsequent sets were disturbed by newly
        // bound pipelineLayout
        return VK_FALSE;
      }
    }
  } else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
    msgPrefix = "INFO";
  } else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
    msgPrefix = "DEBUG";
  } else {
    assert(false);
  }

  // MsgCode seem to be always 0
  LOG_WARN(LogVulkanRHI, "*** [{}] Obj 0x{} Loc {} {}", msgPrefix,
           (void *)srcObject, (uint32_t)location, msg);

  return VK_FALSE;
}

void VulkanRHI::setupDebugLayerCallback() {
  if (activeDebugLayerExtension == DebugLayerExtension::DebugUtils) {
    PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerEXT =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            m_instance, "vkCreateDebugUtilsMessengerEXT");

    if (createDebugUtilsMessengerEXT) {
      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      createInfo.sType =
          VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity =
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugUtilCallback;
      createInfo.pNext = nullptr;
      assert(VK_SUCCESS == createDebugUtilsMessengerEXT(m_instance, &createInfo,
                                                        nullptr, nullptr));
    }
  } else if (activeDebugLayerExtension == DebugLayerExtension::DebugReport) {
    PFN_vkCreateDebugReportCallbackEXT createMsgCallback =
        (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
            m_instance, "vkCreateDebugReportCallbackEXT");
    if (createMsgCallback) {
      VkDebugReportCallbackCreateInfoEXT createInfo;
      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
      createInfo.pNext = nullptr;
      createInfo.flags = VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                         VK_DEBUG_REPORT_WARNING_BIT_EXT |
                         VK_DEBUG_REPORT_ERROR_BIT_EXT;
      createInfo.pfnCallback = debugReportCallback;
      VkResult result =
          createMsgCallback(m_instance, &createInfo, nullptr, nullptr);
      if (result == VK_ERROR_OUT_OF_HOST_MEMORY) {
        LOG_WARN(LogVulkanRHI,
                 "CreateMsgCallback: out of host memory/CreateMsgCallback "
                 "Failure; debug reporting skipped");
      } else if (result != VK_SUCCESS) {
        LOG_WARN(LogVulkanRHI,
                 "CreateMsgCallback: unknown failure {}/CreateMsgCallback "
                 "Failure; debug reporting skipped",
                 result);
      }
    }
    LOG_WARN(LogVulkanRHI, "GetProcAddr: Unable to find "
                           "vkDbgCreateMsgCallback/vkGetInstanceProcAddr; "
                           "debug reporting skipped!");
  }
}

bool VulkanRHI::isDeviceSuitable(const VulkanDevice &physicalDevice) {
  return true;
}

std::vector<LayerWithExtensions>
VulkanRHI::enumerateLayerProperties() {
  std::vector<LayerWithExtensions> ret;
  std::vector<VkLayerProperties> layerProperties;

  uint32_t count = 0;
  {
    VkResult result = vkEnumerateInstanceLayerProperties(&count, nullptr);
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateInstanceLayerProperties", __FILE__, __LINE__);
    }
  }

  layerProperties.resize(count);
  ret.resize(count);
  {
    VkResult result =
        vkEnumerateInstanceLayerProperties(&count, layerProperties.data());
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateInstanceLayerProperties", __FILE__, __LINE__);
    }
  }

  for (int i = 0; i < count; ++i) {
    ret[i].layerProperties = layerProperties[i];
    ret[i].extensionProperties =
        getDriverSupportedInstanceExtensions(layerProperties[i].layerName);
  }

  return ret;
}

void VulkanRHI::createBuffer(uint32_t size, RHIBufferUsageFlags usage,
                             RHIMemoryPropertyFlags property, VkBuffer &buffer,
                             VkDeviceMemory &memory) {
  VkBufferCreateInfo bufferCreateInfo;
  bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferCreateInfo.pNext = nullptr;
  bufferCreateInfo.size = size;
  bufferCreateInfo.usage = static_cast<uint32_t>(usage);
  bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  auto device = m_device.lock();
  if (device == nullptr) {
    return;
  }

  if (vkCreateBuffer(device->getDevice(), &bufferCreateInfo, nullptr,
                     &buffer) != VK_SUCCESS) {
    LOG_ERROR(LogVulkanRHI, "vkCreateBuffer failed!");
    return;
  }

  VkMemoryRequirements requirements;
  vkGetBufferMemoryRequirements(device->getDevice(), buffer, &requirements);

  VkMemoryAllocateInfo allocateInfo;
  allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocateInfo.allocationSize = requirements.size;
  allocateInfo.memoryTypeIndex = device->findMemoryType(
      requirements.memoryTypeBits, static_cast<uint32_t>(property));

  if (vkAllocateMemory(device->getDevice(), &allocateInfo, nullptr, &memory) !=
      VK_SUCCESS) {
    LOG_ERROR(LogVulkanRHI, "vkAllocate failed!");
    return;
  }

  vkBindBufferMemory(device->getDevice(), buffer, memory, 0);
}

std::vector<VkExtensionProperties>
VulkanRHI::getDriverSupportedInstanceExtensions(const char *layerName) {
  std::vector<VkExtensionProperties> ret;

  uint32_t count = 0;
  {
    VkResult result =
        vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateInstanceExtensionProperties", __FILE__, __LINE__);
    }
  }

  ret.resize(count);
  {
    VkResult result =
        vkEnumerateInstanceExtensionProperties(layerName, &count, ret.data());
    if (result == VK_ERROR_INITIALIZATION_FAILED) {
      LOG_ERROR(
          LogVulkanRHI,
          "{} failed\n at {}:{}\nThis typically means Vulkan is not properly "
          "set "
          "up in your system; try running vulkaninfo from the Vulkan SDK.",
          "vkEnumerateInstanceExtensionProperties", __FILE__, __LINE__);
    }
  }

  return ret;
}

} // namespace GEngine