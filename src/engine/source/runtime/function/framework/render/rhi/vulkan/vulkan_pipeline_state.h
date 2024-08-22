#pragma once

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"
#include <array>
#include <cstdint>
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
  std::unordered_map<uint64_t, PoolsList> m_poolsListMap;
};

class VulkanComputePipelineDescriptorState {
public:
  VulkanComputePipelineDescriptorState(
      VulkanDevice *device,
      std::shared_ptr<VulkanRHIComputePipelineState> &computePipelineState,
      const void *parametersData);

  ~VulkanComputePipelineDescriptorState();

  void bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                      m_computePipelineState->getPipeline());
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                            m_computePipelineState->getPipelineLayout(), 0, 1,
                            &m_descriptorSet, 0, nullptr);
  }

  void updateDescriptorSets(const void *parametersData);

private:
  VulkanDevice *m_device;
  std::shared_ptr<VulkanRHIComputePipelineState> m_computePipelineState;
  VkDescriptorSet m_descriptorSet;
  std::vector<VkWriteDescriptorSet> m_descriptorWrites;
  std::vector<VkDescriptorBufferInfo> m_uniformBufferInfos;
  std::vector<VkDescriptorBufferInfo> m_storageBufferInfos;
  std::vector<std::shared_ptr<VulkanRHIUniformBuffer>> m_uniformBuffers;
};

class VulkanGraphicsPipelineDescriptorState {
public:
  VulkanGraphicsPipelineDescriptorState(
      VulkanDevice *device,
      std::shared_ptr<VulkanRHIGraphicsPipelineState> &graphicsPipelineState,
      const void *parametersData);

  ~VulkanGraphicsPipelineDescriptorState();

  void bind(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      m_graphicsPipelineState->getPipeline());
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_graphicsPipelineState->getPipelineLayout(), 0, 1,
                            &m_descriptorSet, 0, nullptr);
  }

  void updateDescriptorSets(const void *parametersData);
private:
  VulkanDevice *m_device;
  std::shared_ptr<VulkanRHIGraphicsPipelineState> m_graphicsPipelineState;
  VkDescriptorSet m_descriptorSet;
  std::vector<VkWriteDescriptorSet> m_descriptorWrites;
  std::vector<VkDescriptorBufferInfo> m_uniformBufferInfos;
  std::vector<VkDescriptorBufferInfo> m_storageBufferInfos;
  std::vector<std::shared_ptr<VulkanRHIUniformBuffer>> m_uniformBuffers;
};

}