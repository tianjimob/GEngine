#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"

namespace GEngine {

class VulkanLayout {
 public:
  VulkanLayout(VulkanDevice* device,
               std::vector<VkDescriptorSetLayout>& setLayouts,
               VkPipelineLayout pipelineLayout,
               std::vector<VkDescriptorSetLayoutBinding>& layoutBindings)
      : m_device(device),
        m_descriptorSetLayouts(setLayouts),
        m_pipelineLayout(pipelineLayout),
        m_layoutBindings(layoutBindings) {}

  uint64_t getLayoutHash() const { return m_layoutHash; }
  uint32_t getTypeUsed(ShaderParametersType type);
  std::vector<VkDescriptorSetLayout>& getSetLayouts() {
    return m_descriptorSetLayouts;
  }
  VkPipelineLayout getPipelineLayout() { return m_pipelineLayout; }

 private:
  VulkanDevice* m_device;

  std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
  VkPipelineLayout m_pipelineLayout;
  std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
  uint64_t m_layoutHash;

  friend class VulkanPipelineStateCacheManager;
};

class VulkanPipelineStateCacheManager {
 public:
  VulkanPipelineStateCacheManager(VulkanDevice* device) : m_device(device) {}

  std::shared_ptr<VulkanRHIComputePipelineState> getOrCreateComputePipeline(
      std::shared_ptr<VulkanRHIComputeShader> computeShader);

  std::shared_ptr<VulkanRHIGraphicsPipelineState> getOrCreateGraphicsPipeline(
      const RHIGraphicsPipelineStateInitiazlier& initializer);

 private:
  VulkanDevice* m_device;

  // hash(shader name) <--> VulkanRHIComputePipelineState
  std::unordered_map<uint64_t, std::shared_ptr<VulkanRHIComputePipelineState>>
      m_computePipelineMap;
  
  // hash(shader name 1 + shader name 2 + ...) <--> VulkanLayout
  std::unordered_map<uint64_t, std::shared_ptr<VulkanLayout>> m_layoutMap;

  std::unordered_map<uint32_t, std::shared_ptr<VulkanRHIGraphicsPipelineState>>
      m_graphicsPipelineMap;

 private:
  std::shared_ptr<VulkanRHIComputePipelineState> createComputePipeline(
      std::shared_ptr<VulkanRHIComputeShader> computeShader);

  std::shared_ptr<VulkanRHIGraphicsPipelineState> createGraphicsPipeline(
      const RHIGraphicsPipelineStateInitiazlier& initializer);

  std::shared_ptr<VulkanLayout> findOrCreateLayout(
      std::shared_ptr<VulkanRHIComputeShader> computeShader);

  std::shared_ptr<VulkanLayout> findOrCreateLayout(
      std::shared_ptr<VulkanRHIVertexShader> vertexShader,
      std::shared_ptr<VulkanRHIPixelShader> pixelShader);

  std::shared_ptr<VulkanLayout>
  createVulkanLayout(std::vector<std::shared_ptr<RHIShader>> &shaders,
                     std::vector<VkShaderStageFlags> &flags);

  uint32_t createGraphicsPipelineKey(const RHIGraphicsPipelineStateInitiazlier& initializer);
};

}  // namespace GEngine