#include "vulkan_pipeline_state.h"

#include "core/log/logger.h"
#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_command_buffer.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_device.h"
#include "vulkan/vulkan_core.h"
#include <cassert>
#include <cstring>
#include <memory>
#include <vector>

namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VkDescriptorSet VulkanDescriptorPoolManager::allocateDescriptorSet(
    std::shared_ptr<VulkanLayout>& vulkanLayout) {
  const uint32_t MaxSets = 128;

  auto createNewPool = [this, &vulkanLayout]() -> VkDescriptorPool {
    std::vector<VkDescriptorPoolSize> poolSizes;
    for (uint32_t type =
             static_cast<uint32_t>(ShaderParametersType::UniformBuffer);
         type < static_cast<uint32_t>(ShaderParametersType::MaxNumType);
         ++type) {
      uint32_t num =
          vulkanLayout->getTypeUsed(static_cast<ShaderParametersType>(type));
      VkDescriptorPoolSize size;
      switch (type) {
        case static_cast<uint32_t>(ShaderParametersType::UniformBuffer):
          size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          break;
        case static_cast<uint32_t>(ShaderParametersType::StorageBuffer):
          size.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
          break;
        default:
          break;
        }
        size.descriptorCount = num * MaxSets;
        poolSizes.emplace_back(size);
    }
    
    VkDescriptorPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.poolSizeCount = poolSizes.size();
    createInfo.pPoolSizes = poolSizes.data();
    createInfo.maxSets = MaxSets;

    VkDescriptorPool pool;
    if (vkCreateDescriptorPool(m_device->getDevice(), &createInfo, nullptr,
                               &pool) != VK_SUCCESS) {
      LOG_ERROR(LogVulkanRHI, "failed create descriptor pool!");
      return VK_NULL_HANDLE;
    } else
      return pool;
  };


  PoolsList* poolList = nullptr;
  if (auto it = m_poolsListMap.find(vulkanLayout->getLayoutName());
      it != m_poolsListMap.end()) {
    poolList = &it->second;
  } else {
    auto [itNew, _] =
        m_poolsListMap.emplace(vulkanLayout->getLayoutName(), PoolsList());
    poolList = &itNew->second;
  }

  auto& pools = *poolList;
  if (pools.empty()) {
    VkDescriptorPool pool = createNewPool();
    if (pool != VK_NULL_HANDLE) pools.emplace_front(pool);
  }

  auto &pool = pools.front();
  VkDescriptorSetAllocateInfo allocateInfo;
  allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocateInfo.descriptorPool = pool;
  allocateInfo.descriptorSetCount = 1;
  allocateInfo.pSetLayouts = &vulkanLayout->getSetLayout();

  VkDescriptorSet descriptorSet;
  VkResult result = vkAllocateDescriptorSets(m_device->getDevice(),
                                             &allocateInfo, &descriptorSet);
  if (result == VK_SUCCESS)
    return descriptorSet;
  else if (result == VK_ERROR_OUT_OF_POOL_MEMORY) {
    VkDescriptorPool pool = createNewPool();
    if (pool != VK_NULL_HANDLE)
      pools.emplace_front(pool);
    else {
      LOG_ERROR(LogVulkanRHI, "failed allocate descriptor set!");
      return nullptr;
    }
  } else {
    LOG_ERROR(LogVulkanRHI, "failed allocate descriptor set!");
    return nullptr;
  }

  pool = pools.front();
  if (vkAllocateDescriptorSets(m_device->getDevice(), &allocateInfo,
                               &descriptorSet) != VK_SUCCESS) {
    LOG_ERROR(LogVulkanRHI, "failed allocate descriptor set!");
    return nullptr;
  }
  return descriptorSet;
}

void VulkanDescriptorPoolManager::tryReleasePool() {
  for (auto &pools : m_PoolsListMap) {
    auto it = pools.begin();
    if (it != pools.end())
      ++it;
    for (; it != pools.end(); ++it) {
      vkDestroyDescriptorPool(m_device->getDevice(), *it, nullptr);
    }
    pools.erase_after(pools.begin(), pools.end());
  }
}

VulkanComputePipelineDescriptorState::VulkanComputePipelineDescriptorState(
    VulkanDevice *device,
    std::shared_ptr<VulkanRHIComputePipelineState> &computePipelineState,
    const void *parametersData)
    : m_computePipelineState(computePipelineState) {
  m_descriptorSet = device->getDescriptorPoolManager().allocateDescriptorSet(computePipelineState->getLayout());
  auto &members = m_computePipelineState->getComputeShader()->getMembers();
  for (auto &member : members) {
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = m_descriptorSet;
    descriptorWrite.dstBinding = member.binding;
    descriptorWrite.dstArrayElement = member.elements;
    descriptorWrite.descriptorCount = 1;
    switch (member.type) {
    case ShaderParametersType::UniformBuffer:
      descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      {
        std::shared_ptr<VulkanRHIUniformBuffer> uniformBuffer =
            std::static_pointer_cast<VulkanRHIUniformBuffer>(
                GlobalRHI->createUniformBuffer(member.size));
        m_uniformBuffers.emplace_back(uniformBuffer);

        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = uniformBuffer->getUniformBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = member.size;
        m_uniformBufferInfos.emplace_back(bufferInfo);
        descriptorWrite.pBufferInfo = &m_uniformBufferInfos.back();
        memcpy(uniformBuffer->getMapped(), (char*)parametersData + member.offset, member.size);
      }
      break;
    case ShaderParametersType::StorageBuffer:
      descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
      {
        RHIBuffer* rhiBuffer = (RHIBuffer*)((char *)parametersData + member.offset);
        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = (VkBuffer)rhiBuffer->getHandle();
        bufferInfo.offset = 0;
        bufferInfo.range = rhiBuffer->size();
        m_storageBufferInfos.emplace_back(bufferInfo);
        descriptorWrite.pBufferInfo = &m_storageBufferInfos.back();
      }
      break;
    default:
      break;
    }
    m_descriptorWrites.emplace_back(descriptorWrite);
  }
  vkUpdateDescriptorSets(device->getDevice(), m_descriptorWrites.size(), m_descriptorWrites.data(), 0, nullptr);
}

}  // namespace GEngine