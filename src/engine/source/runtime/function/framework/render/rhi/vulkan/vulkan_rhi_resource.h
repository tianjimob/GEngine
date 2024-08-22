#pragma once

#include "SDL2/SDL_vulkan.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_sync.h"
#include "function/framework/ui/window.h"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <vector>
namespace GEngine {

class VulkanDevice;
class VulkanLayout;

class VulkanMemory {
public:
  VulkanMemory(VulkanDevice *device, VkDeviceMemory memory)
      : m_vulkanDevice(device), m_memory(memory) {}
  VulkanMemory(VulkanMemory &other) = default;

  void free();

  VkDeviceMemory getMemory() { return m_memory; }
private:
  VulkanDevice *m_vulkanDevice;
  VkDeviceMemory m_memory;
};

class VulkanRHIBuffer : public RHIBuffer {
public:
 VulkanRHIBuffer(uint32_t size, VulkanDevice *device, VkBuffer buffer,
                 VulkanMemory &memory)
     : RHIBuffer(size),
       m_vulkanDevice(device),
       m_buffer(buffer),
       m_vulkanMemory(memory) {}

 ~VulkanRHIBuffer();

 virtual void *getHandle() override { return m_buffer; }
 VkDeviceMemory getMemory() { return m_vulkanMemory.getMemory(); }

private:
  VulkanDevice *m_vulkanDevice;
  VkBuffer m_buffer;
  VulkanMemory m_vulkanMemory;

  friend class VulkanRHI;
};

class VulkanRHIUniformBuffer : public RHIUniformBuffer {
public:
  VulkanRHIUniformBuffer(VulkanDevice *device, VkBuffer buffer,
                         VulkanMemory &memory, void *mapped)
      : m_vulkanDevice(device), m_buffer(buffer), m_vulkanMemory(memory),
        m_mapped(mapped) {}

  ~VulkanRHIUniformBuffer();

  VkBuffer getUniformBuffer() { return m_buffer; }
  void* getMapped() { return m_mapped; }

private:
  VulkanDevice *m_vulkanDevice;
  VkBuffer m_buffer;
  VulkanMemory m_vulkanMemory;
  void* m_mapped;
};

class VulkanRHIComputeShader : public RHIComputeShader {
public:
 VulkanRHIComputeShader(VulkanDevice *device, VkShaderModule shaderModule)
     : m_device(device), m_shaderModule(shaderModule) {}

 virtual ~VulkanRHIComputeShader();
 VkShaderModule& getVkShaderModule() { return m_shaderModule; }

 private:
  VulkanDevice* m_device;
  VkShaderModule m_shaderModule;

  friend class VulkanRHI;
};

class VulkanRHIComputePipelineState : public RHIComputePipelineState {
public:
 VulkanRHIComputePipelineState(
     VulkanDevice *device,
     std::shared_ptr<VulkanRHIComputeShader> &computeShader,
     VkPipeline pipeline, std::shared_ptr<VulkanLayout> vulkanLayout)
     : m_vulkanDevice(device),
       m_computeShader(computeShader),
       m_pipeline(pipeline),
       m_vulkanLayout(vulkanLayout) {}

 auto &getComputeShader() { return m_computeShader; }

 std::shared_ptr<VulkanLayout> &getLayout() { return m_vulkanLayout; }
 VkPipeline getPipeline() { return m_pipeline; }
 VkPipelineLayout getPipelineLayout();

private:
 VulkanDevice *m_vulkanDevice;
 std::shared_ptr<VulkanRHIComputeShader> m_computeShader;
 VkPipeline m_pipeline;
 std::shared_ptr<VulkanLayout> m_vulkanLayout;
};

class VulkanRHIVertexShader : public RHIVertexShader {
public:
 VulkanRHIVertexShader(VulkanDevice *device, VkShaderModule shaderModule)
     : m_device(device), m_shaderModule(shaderModule) {}

 virtual ~VulkanRHIVertexShader();
 VkShaderModule& getVkShaderModule() { return m_shaderModule; }

 private:
  VulkanDevice* m_device;
  VkShaderModule m_shaderModule;
};

class VulkanRHIPixelShader : public RHIPixelShader {
public:
 VulkanRHIPixelShader(VulkanDevice *device, VkShaderModule shaderModule)
     : m_device(device), m_shaderModule(shaderModule) {}

 virtual ~VulkanRHIPixelShader();
 VkShaderModule& getVkShaderModule() { return m_shaderModule; }

 private:
  VulkanDevice* m_device;
  VkShaderModule m_shaderModule;
};

struct VulkanRHIRasterizerState : public RHIRasterizerState {
  VkPolygonMode polygonMode;
  VkCullModeFlags cullMode;
};

struct VulkanRHIDepthStencilState : public RHIDepthStencilState {
  bool enableDepthTest;
  CompareOperator depthCompareOp;
  bool enableDepthWrite;
  bool enableStencilTest;
  StencilOperator frontFailOp;
  StencilOperator frontPassOp;
  StencilOperator frontDepthFailOp;
  CompareOperator frontCompareOp;
  StencilOperator backFailOp;
  StencilOperator backPassOp;
  StencilOperator backDepthFailOp;
  CompareOperator backCompareOp;
};

struct VulkanRHIBlendState : public RHIBlendState {
  std::vector<VkPipelineColorBlendAttachmentState> blendStates;
};

class VulkanRHIGraphicsPipelineState : public RHIGraphicsPipelineState {
public:
 VulkanRHIGraphicsPipelineState(VulkanDevice *device, VkPipeline pipeline,
                                std::shared_ptr<VulkanLayout> &layout,
                                std::shared_ptr<RHIVertexShader> &vertexShader,
                                std::shared_ptr<RHIPixelShader> &pixelShader)
     : m_device(device),
       m_pipeline(pipeline),
       m_vulkanLayout(layout),
       m_shaders(2) {
   m_shaders[0] = std::static_pointer_cast<RHIShader>(vertexShader);
   m_shaders[1] = std::static_pointer_cast<RHIShader>(pixelShader);
 }

 std::shared_ptr<VulkanLayout> &getLayout() { return m_vulkanLayout; }
 VkPipeline getPipeline() { return m_pipeline; }
 VkPipelineLayout getPipelineLayout();
 std::vector<std::shared_ptr<RHIShader>>& getShaders() { return m_shaders; }

private:
  VulkanDevice *m_device;
  VkPipeline m_pipeline;
  std::shared_ptr<VulkanLayout> m_vulkanLayout;
  // [0] is vertex shader, [1] is pixel shader
  std::vector<std::shared_ptr<RHIShader>> m_shaders;
};

class VulkanSwapChain {
public:
  VulkanSwapChain(Window* window, VkInstance instance,
                  std::shared_ptr<VulkanDevice> vulkanDevice);
  ~VulkanSwapChain();

private:
  VulkanDevice& m_device;
  VkSurfaceKHR m_surface;

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapChainSupportDetails swapChainSupport;
  VkSurfaceFormatKHR m_surfaceFormat;
  VkPresentModeKHR m_presentMode;
  VkExtent2D m_extent;
  VkSwapchainKHR m_swapChain;
  std::vector<VkImage> m_swapChainImages;
  std::vector<VkImageView> m_swapChainImageViews;
  std::vector<std::shared_ptr<VulkanSemaphore>> m_imageAcquiredSemaphore;

private:
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);
      
};

class VulkanRHIViewport : public RHIViewport {
public:
  VulkanRHIViewport(Window* window);

private:
  std::shared_ptr<VulkanSwapChain> m_swapChain;
};

}