#include "vulkan_pipeline.h"

#include <memory>
#include <vector>

#include "core/log/logger.h"
#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/vulkan/vulkan_device.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"

namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

uint32_t VulkanLayout::getTypeUsed(ShaderParametersType type) {
  int num = 0;
  for (auto &layoutBinding : m_layoutBindings) {
    switch (type) {
    case ShaderParametersType::UniformBuffer:
      if (layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
        ++num;
    case ShaderParametersType::StorageBuffer:
      if (layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
        ++num;
    default:
      break;
    }
  }
  return num;
}

std::shared_ptr<VulkanRHIComputePipelineState>
VulkanPipelineStateCacheManager::getOrCreateComputePipeline(
    std::shared_ptr<VulkanRHIComputeShader> computeShader) {
  if (auto it = m_computePipelineMap.find(computeShader->getShaderName());
      it != m_computePipelineMap.end()) {
    return it->second;
  } else {
    std::shared_ptr<VulkanRHIComputePipelineState> pipeline =
        createComputePipeline(computeShader);
    if(pipeline == nullptr) return nullptr;
    m_computePipelineMap[computeShader->getShaderName()] = pipeline;
    return pipeline;
  }
}

std::shared_ptr<VulkanRHIComputePipelineState>
VulkanPipelineStateCacheManager::createComputePipeline(
    std::shared_ptr<VulkanRHIComputeShader> computeShader) {

  std::shared_ptr<VulkanLayout> layout = findOrCreateLayout(computeShader);
  if(layout == nullptr) return nullptr;
  
  VkComputePipelineCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  createInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  createInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
  createInfo.stage.module = computeShader->getVkShaderModule();
  createInfo.stage.pName = "main";
  createInfo.layout = layout->m_pipelineLayout;

  VkPipeline pipeline;
  // todo: pipeline cache -- FVulkanPipelineStateCacheManager::Load
  if (vkCreateComputePipelines(m_device->getDevice(), VK_NULL_HANDLE, 1,
                               &createInfo, nullptr, &pipeline) != VK_SUCCESS) {
    LOG_ERROR(LogVulkanRHI, "failed to create compute pipeline");
    return nullptr;
  }

  return std::make_shared<VulkanRHIComputePipelineState>(
      m_device, computeShader, pipeline, layout);
}

std::shared_ptr<VulkanLayout>
VulkanPipelineStateCacheManager::findOrCreateLayout(
    std::shared_ptr<VulkanRHIComputeShader> computeShader) {
  if (auto it = m_layoutMap.find(computeShader->getShaderName());
      it != m_layoutMap.end()) {
    return it->second;
  } else {
    auto members = computeShader->getMembers();
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
    for (auto &member : members) {
      VkDescriptorSetLayoutBinding binding;
      binding.binding = member.binding;
      binding.descriptorCount = 1;
      binding.pImmutableSamplers = nullptr;
      binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
      switch (member.type) {
        case ShaderParametersType::UniformBuffer:
          binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          break;
        case ShaderParametersType::StorageBuffer:
          binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
          break;
        default:
          break;
      }
      layoutBindings.emplace_back(binding);
    }

    VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo;
    setLayoutCreateInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setLayoutCreateInfo.bindingCount = layoutBindings.size();
    setLayoutCreateInfo.pBindings = layoutBindings.data();

    VkDescriptorSetLayout setLayout;
    if (vkCreateDescriptorSetLayout(m_device->getDevice(), &setLayoutCreateInfo,
                                    nullptr, &setLayout) != VK_SUCCESS) {
      LOG_ERROR(LogVulkanRHI, "failed create descriptor set layout!");
      return nullptr;
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &setLayout;

    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(m_device->getDevice(), &pipelineLayoutCreateInfo,
                               nullptr, &pipelineLayout) != VK_SUCCESS) {
      LOG_ERROR(LogVulkanRHI, "failed create pipeline layout");
      vkDestroyDescriptorSetLayout(m_device->getDevice(), setLayout, nullptr);
      return nullptr;
    }

    std::shared_ptr<VulkanLayout> layout =
        std::make_shared<VulkanLayout>(m_device, setLayout, pipelineLayout, layoutBindings);
    m_layoutMap[computeShader->getShaderName()] = layout;
    return layout;
  }
}

}  // namespace GEngine