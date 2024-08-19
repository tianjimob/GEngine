#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
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
                                std::shared_ptr<VulkanLayout> &layout)
     : m_device(device), m_pipeline(pipeline), m_vulkanLayout(layout) {}

private:
  VulkanDevice *m_device;
  VkPipeline m_pipeline;
  std::shared_ptr<VulkanLayout> m_vulkanLayout;
};

}