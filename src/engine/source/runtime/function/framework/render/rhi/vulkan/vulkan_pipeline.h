#pragma once

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace GEngine {

class VulkanLayout {
public:
 VulkanLayout(VulkanDevice* device, VkDescriptorSetLayout setLayout,
              VkPipelineLayout pipelineLayout,
              std::vector<VkDescriptorSetLayoutBinding>& layoutBindings)
     : m_device(device),
       m_descriptorSetLayout(setLayout),
       m_pipelineLayout(pipelineLayout),
       m_layoutBindings(layoutBindings) {}

 const std::string getLayoutName() const { return m_layoutName; }
 uint32_t getTypeUsed(ShaderParametersType type);
 VkDescriptorSetLayout &getSetLayout() { return m_descriptorSetLayout; }
 VkPipelineLayout getPipelineLayout() { return m_pipelineLayout; }
private:
  VulkanDevice *m_device;

  VkDescriptorSetLayout m_descriptorSetLayout;
  VkPipelineLayout m_pipelineLayout;
  std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
  std::string m_layoutName;

  friend class VulkanPipelineStateCacheManager;
};

class VulkanPipelineStateCacheManager {
public:
 VulkanPipelineStateCacheManager(VulkanDevice* device) : m_device(device) {}

 std::shared_ptr<VulkanRHIComputePipelineState> getOrCreateComputePipeline(
     std::shared_ptr<VulkanRHIComputeShader> computeShader);
  
private:
  VulkanDevice* m_device;
  std::unordered_map<std::string,
                     std::shared_ptr<VulkanRHIComputePipelineState>>
      m_computePipelineMap;
  std::unordered_map<std::string, std::shared_ptr<VulkanLayout>> m_layoutMap;

 private:
   std::shared_ptr<VulkanRHIComputePipelineState>
   createComputePipeline(std::shared_ptr<VulkanRHIComputeShader> computeShader);
   std::shared_ptr<VulkanLayout> findOrCreateLayout(std::shared_ptr<VulkanRHIComputeShader> computeShader);
};

}