#include "vulkan_pipeline.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "CRC.h"
#include "core/log/logger.h"
#include "function/framework/render/core/shaders.h"
#include "function/framework/render/mesh_pipeline/vertex_streams.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_device.h"
#include "function/framework/render/rhi/vulkan/vulkan_macros.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_utils.h"
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
        break;
      case ShaderParametersType::StorageBuffer:
        if (layoutBinding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
          ++num;
        break;
      default:
        break;
    }
  }
  return num;
}

std::shared_ptr<VulkanRHIComputePipelineState>
VulkanPipelineStateCacheManager::getOrCreateComputePipeline(
    std::shared_ptr<VulkanRHIComputeShader> computeShader) {
  if (auto it = m_computePipelineMap.find(
          std::hash<std::string>{}(computeShader->getShaderName()));
      it != m_computePipelineMap.end()) {
    return it->second;
  } else {
    std::shared_ptr<VulkanRHIComputePipelineState> pipeline =
        createComputePipeline(computeShader);
    if (pipeline == nullptr) return nullptr;
    m_computePipelineMap[std::hash<std::string>{}(
        computeShader->getShaderName())] = pipeline;
    return pipeline;
  }
}

std::shared_ptr<VulkanRHIGraphicsPipelineState>
VulkanPipelineStateCacheManager::getOrCreateGraphicsPipeline(
    const RHIGraphicsPipelineStateInitiazlier &initializer) {
  uint32_t key = createGraphicsPipelineKey(initializer);
  if (auto it = m_graphicsPipelineMap.find(key);
      it != m_graphicsPipelineMap.end()) {
    return it->second;
  } else {
    std::shared_ptr<VulkanRHIGraphicsPipelineState> pipeline =
        createGraphicsPipeline(initializer);
    if (pipeline == nullptr) return nullptr;
    m_graphicsPipelineMap[key] = pipeline;
    return pipeline;
  }
}

std::shared_ptr<VulkanRHIGraphicsPipelineState>
VulkanPipelineStateCacheManager::createGraphicsPipeline(
    const RHIGraphicsPipelineStateInitiazlier &initializer) {
  assert(initializer.shaderState.vertexShader);
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
  if (initializer.shaderState.vertexShader) {
    auto &stage = shaderStages.emplace_back();
    ZERO_VULKAN_STRUCT(stage);
    stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    stage.module = std::static_pointer_cast<VulkanRHIVertexShader>(
                       initializer.shaderState.vertexShader)
                       ->getVkShaderModule();
    stage.pName = "main";
  }
  if (initializer.shaderState.pixelShader) {
    auto &stage = shaderStages.emplace_back();
    ZERO_VULKAN_STRUCT(stage);
    stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stage.module = std::static_pointer_cast<VulkanRHIPixelShader>(
                       initializer.shaderState.pixelShader)
                       ->getVkShaderModule();
    stage.pName = "main";
  }

  VkPipelineVertexInputStateCreateInfo vertexInputInfo;
  ZERO_VULKAN_STRUCT(vertexInputInfo);
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  std::vector<VkVertexInputBindingDescription> inputBindings;
  std::vector<VkVertexInputAttributeDescription> inputAttributes;
  {
    int bindingCount = 0;
    for (const VertexStream &vertexStream :
         initializer.vertexStreams.getVertexStreams()) {
      auto &inputBinding = inputBindings.emplace_back();
      inputBinding.binding = bindingCount;
      inputBinding.stride = vertexStream.stride;
      inputBinding.stride = VK_VERTEX_INPUT_RATE_VERTEX;

      auto &inputAttribute = inputAttributes.emplace_back();
      inputAttribute.binding = bindingCount;
      inputAttribute.location = bindingCount++;
      inputAttribute.offset = 0;
      inputAttribute.format =
          VulkanUtils::getVkFormat(vertexStream.elementType);
    }
  }
  vertexInputInfo.vertexBindingDescriptionCount = inputBindings.size();
  vertexInputInfo.pVertexBindingDescriptions = inputBindings.data();
  vertexInputInfo.vertexAttributeDescriptionCount = inputAttributes.size();
  vertexInputInfo.pVertexAttributeDescriptions = inputAttributes.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly;
  ZERO_VULKAN_STRUCT(inputAssembly);
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology =
      VulkanUtils::getVkPrimitiveTopology(initializer.primitiveType);
  inputAssembly.primitiveRestartEnable = false;

  VkPipelineViewportStateCreateInfo viewportState;
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer;
  ZERO_VULKAN_STRUCT(rasterizer);
  auto vulkanRasterizer = std::static_pointer_cast<VulkanRHIRasterizerState>(
      initializer.rasterizerState);
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.polygonMode = vulkanRasterizer->polygonMode;
  rasterizer.cullMode = vulkanRasterizer->cullMode;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

  VkPipelineMultisampleStateCreateInfo multisampling;
  ZERO_VULKAN_STRUCT(multisampling);
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.rasterizationSamples =
      (VkSampleCountFlagBits)initializer.numSamples;

  VkPipelineDepthStencilStateCreateInfo depthStencil;
  ZERO_VULKAN_STRUCT(depthStencil);
  auto &vulkanDepthStencilState =
      *std::static_pointer_cast<VulkanRHIDepthStencilState>(
          initializer.depthStencilState);
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = vulkanDepthStencilState.enableDepthTest;
  depthStencil.depthCompareOp =
      static_cast<VkCompareOp>(vulkanDepthStencilState.depthCompareOp);
  depthStencil.depthWriteEnable = vulkanDepthStencilState.enableDepthWrite;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.stencilTestEnable = vulkanDepthStencilState.enableStencilTest;
  if (vulkanDepthStencilState.enableStencilTest) {
    depthStencil.front.failOp =
        static_cast<VkStencilOp>(vulkanDepthStencilState.frontFailOp);
    depthStencil.front.passOp =
        static_cast<VkStencilOp>(vulkanDepthStencilState.frontPassOp);
    depthStencil.front.depthFailOp =
        static_cast<VkStencilOp>(vulkanDepthStencilState.frontDepthFailOp);
    depthStencil.front.compareOp =
        static_cast<VkCompareOp>(vulkanDepthStencilState.frontCompareOp);
    depthStencil.front.compareMask = 0xFF;
    depthStencil.front.writeMask = 0xFF;
    depthStencil.front.reference = 0;

    depthStencil.back.failOp =
        static_cast<VkStencilOp>(vulkanDepthStencilState.backFailOp);
    depthStencil.back.passOp =
        static_cast<VkStencilOp>(vulkanDepthStencilState.backPassOp);
    depthStencil.back.depthFailOp =
        static_cast<VkStencilOp>(vulkanDepthStencilState.backDepthFailOp);
    depthStencil.back.compareOp =
        static_cast<VkCompareOp>(vulkanDepthStencilState.backCompareOp);
    depthStencil.back.compareMask = 0xFF;
    depthStencil.back.writeMask = 0xFF;
    depthStencil.back.reference = 0;
  }

  VkPipelineColorBlendStateCreateInfo blendState;
  ZERO_VULKAN_STRUCT(blendState);
  auto &vulkanBlendStates =
      std::static_pointer_cast<VulkanRHIBlendState>(initializer.blendState)
          ->blendStates;
  blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  blendState.attachmentCount = vulkanBlendStates.size();
  blendState.pAttachments = vulkanBlendStates.data();
  blendState.blendConstants[0] = 1.f;
  blendState.blendConstants[1] = 1.f;
  blendState.blendConstants[2] = 1.f;
  blendState.blendConstants[3] = 1.f;

  VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                    VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState;
  ZERO_VULKAN_STRUCT(dynamicState);
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount =
      sizeof(dynamicStates) / sizeof(VkDynamicState);
  dynamicState.pDynamicStates = dynamicStates;

  std::shared_ptr<VulkanLayout> layout =
      findOrCreateLayout(std::static_pointer_cast<VulkanRHIVertexShader>(
                             initializer.shaderState.vertexShader),
                         std::static_pointer_cast<VulkanRHIPixelShader>(
                             initializer.shaderState.pixelShader));

  VkGraphicsPipelineCreateInfo pipelineCreateInfo;
  ZERO_VULKAN_STRUCT(pipelineCreateInfo);
  pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineCreateInfo.stageCount = shaderStages.size();
  pipelineCreateInfo.pStages = shaderStages.data();
  pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
  pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
  pipelineCreateInfo.pViewportState = &viewportState;
  pipelineCreateInfo.pRasterizationState = &rasterizer;
  pipelineCreateInfo.pMultisampleState = &multisampling;
  pipelineCreateInfo.pDepthStencilState = &depthStencil;
  pipelineCreateInfo.pColorBlendState = &blendState;
  pipelineCreateInfo.pDynamicState = &dynamicState;
  pipelineCreateInfo.layout = layout->getPipelineLayout();
  pipelineCreateInfo.renderPass = m_device->getRenderPassManager()
                                      .getOrCreateRenderPass(initializer)
                                      ->getVkRenderPass();
  pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineCreateInfo.subpass = 0;

  VkPipeline pipeline;
  if (vkCreateGraphicsPipelines(m_device->getDevice(), VK_NULL_HANDLE, 1,
                                &pipelineCreateInfo, nullptr,
                                &pipeline) != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "vkCreateGraphicsPipelines failed!");
  }

  std::shared_ptr<VulkanRHIGraphicsPipelineState> vulkanPipeline =
      std::make_shared<VulkanRHIGraphicsPipelineState>(m_device, pipeline,
                                                       layout);
  return vulkanPipeline;
}

std::shared_ptr<VulkanRHIComputePipelineState>
VulkanPipelineStateCacheManager::createComputePipeline(
    std::shared_ptr<VulkanRHIComputeShader> computeShader) {
  std::shared_ptr<VulkanLayout> layout = findOrCreateLayout(computeShader);
  if (layout == nullptr) return nullptr;

  VkComputePipelineCreateInfo createInfo;
  ZERO_VULKAN_STRUCT(createInfo);
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
  if (auto it = m_layoutMap.find(
          std::hash<std::string>{}(computeShader->getShaderName()));
      it != m_layoutMap.end()) {
    return it->second;
  } else {
    std::vector<std::shared_ptr<RHIShader>> shaders{
        std::static_pointer_cast<RHIShader>(computeShader)};
    std::vector<VkShaderStageFlags> flags{VK_SHADER_STAGE_COMPUTE_BIT};
    std::shared_ptr<VulkanLayout> layout = createVulkanLayout(shaders, flags);
    m_layoutMap[std::hash<std::string>{}(computeShader->getShaderName())] =
        layout;
    return layout;
  }
}

std::shared_ptr<VulkanLayout>
VulkanPipelineStateCacheManager::findOrCreateLayout(
    std::shared_ptr<VulkanRHIVertexShader> vertexShader,
    std::shared_ptr<VulkanRHIPixelShader> pixelShader) {
  std::string hashStr;
  if (vertexShader) hashStr += vertexShader->getShaderName();
  if (pixelShader) hashStr += pixelShader->getShaderName();

  uint64_t key = std::hash<std::string>{}(hashStr);
  if (auto it = m_layoutMap.find(key); it != m_layoutMap.end()) {
    return it->second;
  } else {
    std::vector<std::shared_ptr<RHIShader>> shaders;
    std::vector<VkShaderStageFlags> flags;
    if (vertexShader) {
      shaders.emplace_back(std::static_pointer_cast<RHIShader>(vertexShader));
      flags.emplace_back(VK_SHADER_STAGE_VERTEX_BIT);
    }
    if (pixelShader) {
      shaders.emplace_back(std::static_pointer_cast<RHIShader>(pixelShader));
      flags.emplace_back(VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    std::shared_ptr<VulkanLayout> layout = createVulkanLayout(shaders, flags);
    m_layoutMap[key] = layout;
    return layout;
  }
}

std::shared_ptr<VulkanLayout>
VulkanPipelineStateCacheManager::createVulkanLayout(
    std::vector<std::shared_ptr<RHIShader>> &shaders,
    std::vector<VkShaderStageFlags> &flags) {
  std::vector<VkDescriptorSetLayout> setLayouts(shaders.size());
  std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
  {
    int shaderCount = 0;
    for (auto &shader : shaders) {
      auto members = shader->getMembers();
      for (auto &member : members) {
        VkDescriptorSetLayoutBinding binding;
        binding.binding = member.binding;
        binding.descriptorCount = 1;
        binding.pImmutableSamplers = nullptr;
        binding.stageFlags = flags[shaderCount];
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
      ZERO_VULKAN_STRUCT(setLayoutCreateInfo);
      setLayoutCreateInfo.sType =
          VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      setLayoutCreateInfo.bindingCount = layoutBindings.size();
      setLayoutCreateInfo.pBindings = layoutBindings.data();

      VkDescriptorSetLayout setLayout;
      if (vkCreateDescriptorSetLayout(m_device->getDevice(),
                                      &setLayoutCreateInfo, nullptr,
                                      &setLayout) != VK_SUCCESS) {
        LOG_ERROR(LogVulkanRHI, "failed create descriptor set layout!");
        return nullptr;
      }

      ++shaderCount;
    }
  }

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
  ZERO_VULKAN_STRUCT(pipelineLayoutCreateInfo);
  pipelineLayoutCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.setLayoutCount = setLayouts.size();
  pipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();

  VkPipelineLayout pipelineLayout;
  if (vkCreatePipelineLayout(m_device->getDevice(), &pipelineLayoutCreateInfo,
                             nullptr, &pipelineLayout) != VK_SUCCESS) {
    LOG_ERROR(LogVulkanRHI, "failed create pipeline layout");
    for (auto &setLayout : setLayouts) {
      vkDestroyDescriptorSetLayout(m_device->getDevice(), setLayout, nullptr);
    }
    return nullptr;
  }

  std::shared_ptr<VulkanLayout> layout = std::make_shared<VulkanLayout>(
      m_device, setLayouts, pipelineLayout, layoutBindings);
}

uint32_t VulkanPipelineStateCacheManager::createGraphicsPipelineKey(
    const RHIGraphicsPipelineStateInitiazlier &initializer) {
  std::vector<uint8_t> initializerHashInfo;
  if (initializer.shaderState.vertexShader) {
    for (auto ch : initializer.shaderState.vertexShader->getShaderName()) {
      initializerHashInfo.emplace_back(ch);
    }
  }

  if (initializer.shaderState.pixelShader) {
    for (auto ch : initializer.shaderState.pixelShader->getShaderName()) {
      initializerHashInfo.emplace_back(ch);
    }
  }

  for (auto &vertexStream : initializer.vertexStreams.getVertexStreams()) {
    for (int i = 0; i < sizeof(VertexStream) - offsetof(VertexStream, stride);
         ++i)
      initializerHashInfo.emplace_back(
          *(reinterpret_cast<const uint8_t *>(&vertexStream.stride) + i));
  }

  if (initializer.rasterizerState) {
    auto vulkanRasterizerState =
        std::static_pointer_cast<VulkanRHIRasterizerState>(
            initializer.rasterizerState);
    for (int i = 0; i < sizeof(VulkanRHIRasterizerState); ++i)
      initializerHashInfo.emplace_back(
          *(reinterpret_cast<const uint8_t *>(&*vulkanRasterizerState) + i));
  }

  if (initializer.depthStencilState) {
    auto vulkanDepthStencilState =
        std::static_pointer_cast<VulkanRHIDepthStencilState>(
            initializer.depthStencilState);
    for (int i = 0; i < sizeof(VulkanRHIDepthStencilState); ++i)
      initializerHashInfo.emplace_back(
          *(reinterpret_cast<const uint8_t *>(&*vulkanDepthStencilState) + i));
  }

  if (initializer.blendState) {
    auto vulkanBlendState =
        std::static_pointer_cast<VulkanRHIBlendState>(initializer.blendState);
    for (auto &blendState : vulkanBlendState->blendStates) {
      for (int i = 0; i < sizeof(blendState); ++i)
        initializerHashInfo.emplace_back(
            *(reinterpret_cast<const uint8_t *>(&blendState) + i));
    }
  }

  for (int i = 0;
       i <
       offsetof(RHIGraphicsPipelineStateInitiazlier, colorAttachmentFormats) -
           offsetof(RHIGraphicsPipelineStateInitiazlier, primitiveType);
       ++i) {
    initializerHashInfo.emplace_back(
        *(reinterpret_cast<const uint8_t *>(&initializer.primitiveType) + i));
  }

  uint32_t renderpassHash = initializer.getRenderPassHash();
  for (int i = 0; i < sizeof(renderpassHash); ++i) {
    initializerHashInfo.emplace_back(
        *(reinterpret_cast<const uint8_t *>(&renderpassHash) + i));
  }

  return CRC::Calculate(initializerHashInfo.data(), initializerHashInfo.size(),
                        CRC::CRC_32());
}

}  // namespace GEngine