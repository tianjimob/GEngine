#include "vulkan_rhi_resource.h"

#include <cassert>
#include <memory>

#include "SDL2/SDL_video.h"
#include "SDL2/SDL_vulkan.h"
#include "core/log/logger.h"
#include "core/math/ivector2.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/vulkan/vulkan_device.h"
#include "function/framework/render/rhi/vulkan/vulkan_macros.h"
#include "function/framework/render/rhi/vulkan/vulkan_pipeline.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi.h"
#include "function/framework/render/rhi/vulkan/vulkan_sync.h"
#include "vulkan/vulkan_core.h"

namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

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

VkPipelineLayout VulkanRHIGraphicsPipelineState::getPipelineLayout() {
  return m_vulkanLayout->getPipelineLayout();
}

VulkanSwapChain::VulkanSwapChain(Window* window, VkInstance instance,
                                 std::shared_ptr<VulkanDevice> vulkanDevice)
    : m_device(*vulkanDevice) {
  SDL_Vulkan_CreateSurface(
      reinterpret_cast<SDL_Window*>(window->getNativeHandle()), instance,
      &m_surface);

  swapChainSupport = querySwapChainSupport(vulkanDevice->getPhysicalDevice());
  m_surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  m_presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  m_extent = chooseSwapExtent(swapChainSupport.capabilities, window);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  ZERO_VULKAN_STRUCT(createInfo);
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = m_surface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = m_surfaceFormat.format;
  createInfo.imageColorSpace = m_surfaceFormat.colorSpace;
  createInfo.imageExtent = m_extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  vulkanDevice->setupPresentQueue(m_surface);
  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = m_presentMode;
  createInfo.clipped = VK_TRUE;

  if (vkCreateSwapchainKHR(vulkanDevice->getDevice(), &createInfo, nullptr,
                           &m_swapChain) != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(vulkanDevice->getDevice(), m_swapChain, &imageCount,
                          nullptr);
  m_swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(vulkanDevice->getDevice(), m_swapChain, &imageCount,
                          m_swapChainImages.data());

  m_imageAcquiredSemaphore.resize(imageCount);
  for (auto& semaphore : m_imageAcquiredSemaphore) {
    semaphore = std::make_shared<VulkanSemaphore>();
  }

  for (size_t i = 0; i < m_swapChainImages.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_swapChainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = m_surfaceFormat.format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(vulkanDevice->getDevice(), &createInfo, nullptr,
                          &m_swapChainImageViews[i]) != VK_SUCCESS) {
      LOG_ERROR(LogVulkanRHI, "failed to create image views!");
    }
  }
}

VulkanSwapChain::~VulkanSwapChain() {
  vkDestroySwapchainKHR(m_device.getDevice(), m_swapChain, nullptr);
}

VulkanRHIViewport::VulkanRHIViewport(Window* window) {
  VulkanRHI* rhi = reinterpret_cast<VulkanRHI*>(GlobalRHI);
  assert(rhi);
  m_swapChain = std::make_shared<VulkanSwapChain>(window, rhi->getInstance(),
                                                  rhi->getActiveDevice());
}

VulkanSwapChain::SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(
    VkPhysicalDevice device) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount,
                                       nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface,
                                            &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, m_surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR& capabilities,
    Window* window) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    IVector2 size = window->getDrawableSize();
    width = size.x;
    height = size.y;

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

}  // namespace GEngine