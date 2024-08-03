#pragma once

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"
#include <array>
#include <forward_list>
#include <memory>
#include <unordered_map>
#include <vector>
namespace GEngine {

class VulkanCommandBuffer;

class VulkanDescriptorPoolManager {
public:
  VulkanDescriptorPoolManager(VulkanDevice* device):m_device(device){}
  VkDescriptorSet allocateDescriptorSet(std::shared_ptr<VulkanLayout>& vulkanLayout);
  void tryReleasePool();
private:
  using PoolsList = std::forward_list<VkDescriptorPool>;
  VulkanDevice* m_device;
  std::array<PoolsList, static_cast<int>(ShaderParametersType::MaxNumType)>
      m_PoolsListMap;
  std::unordered_map<std::string, PoolsList> m_poolsListMap;
};

class VulkanComputePipelineDescriptorState {
public:
  VulkanComputePipelineDescriptorState(
      VulkanDevice *device,
      std::shared_ptr<VulkanRHIComputePipelineState> &computePipelineState,
      const void *parametersData);

  void bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                      m_computePipelineState->getPipeline());
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                            m_computePipelineState->getPipelineLayout(), 0, 1,
                            &m_descriptorSet, 0, nullptr);
  }
private:
  std::shared_ptr<VulkanRHIComputePipelineState> m_computePipelineState;
  VkDescriptorSet m_descriptorSet;
  std::vector<VkWriteDescriptorSet> m_descriptorWrites;
  std::vector<VkDescriptorBufferInfo> m_uniformBufferInfos;
  std::vector<VkDescriptorBufferInfo> m_storageBufferInfos;
  std::vector<std::shared_ptr<VulkanRHIUniformBuffer>> m_uniformBuffers;

};

}