#include "vulkan_render_pass.h"

#include <cstring>
#include <memory>

#include "core/log/logger.h"
#include "function/framework/render/rhi/rhi_type.h"
#include "function/framework/render/rhi/vulkan/vulkan_device.h"
#include "function/framework/render/rhi/vulkan/vulkan_macros.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_utils.h"
#include "vulkan/vulkan_core.h"


namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VulkanRenderPass::VulkanRenderPass(
    VulkanDevice *device,
    const RHIGraphicsPipelineStateInitiazlier &initializer)
    : m_device(device),
      attachments(initializer.getAttachmentsSize()),
      colorRenferences(initializer.colorAttachmentFormats.size()),
      resolveReferences(initializer.numSamples == 1 ? 0
                                                    : colorRenferences.size()) {
  memset(attachments.data(), 0,
         attachments.size() * sizeof(VkAttachmentDescription));

  int attachmentCount = 0;
  {
    int colorCount = initializer.colorAttachmentFormats.size();
    for (int i = 0; i < colorCount; ++i) {
      auto &desc = attachments[attachmentCount];
      desc.samples = static_cast<VkSampleCountFlagBits>(initializer.numSamples);
      desc.format =
          static_cast<VkFormat>(initializer.colorAttachmentFormats[i]);
      desc.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      desc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      desc.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      desc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      colorRenferences[i].attachment = attachmentCount++;
      colorRenferences[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      if (desc.samples != VK_SAMPLE_COUNT_1_BIT) {
        auto &resolveDesc = attachments[attachmentCount];
        resolveDesc.samples =
            static_cast<VkSampleCountFlagBits>(initializer.numSamples);
        resolveDesc.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        resolveDesc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        resolveDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        resolveDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        resolveDesc.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        resolveDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        resolveReferences[i].attachment = attachmentCount++;
        resolveReferences[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      }
    }
  }

  if (initializer.hasDepthAttachment) {
    auto &desc = attachments[attachmentCount];
    desc.samples = static_cast<VkSampleCountFlagBits>(initializer.numSamples);
    desc.format = static_cast<VkFormat>(initializer.depthAttachmentFormat);
    desc.loadOp = VulkanUtils::getVkAttachmentLoadOp(initializer.depthLoadOp);
    desc.stencilLoadOp =
        VulkanUtils::getVkAttachmentLoadOp(initializer.stencilLoadOp);
    if (desc.samples == VK_SAMPLE_COUNT_1_BIT) {
      desc.storeOp =
          VulkanUtils::getVkAttachmentStoreOp(initializer.depthStoreOp);
      desc.stencilStoreOp =
          VulkanUtils::getVkAttachmentStoreOp(initializer.stencilStoreOp);
    } else {
      // Never want to store MSAA depth/stencil
      desc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
    desc.initialLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    desc.finalLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;

    depthReference.attachment = attachmentCount++;
    depthReference.layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
  }

  // main sub-pass
  {
    auto &subpass = subpassDescriptions.emplace_back();
    ZERO_VULKAN_STRUCT(subpass);
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = colorRenferences.size();
    subpass.pColorAttachments = colorRenferences.data();
    if (initializer.subpassType != SubpassType::DepthRead) {
      subpass.pResolveAttachments = resolveReferences.data();
    }
    if (initializer.hasDepthAttachment) {
      subpass.pDepthStencilAttachment = &depthReference;
    }
  }

  // Color write and depth read sub-pass
  if (initializer.subpassType == SubpassType::DepthRead) {
    auto &subpass = subpassDescriptions.emplace_back();
    ZERO_VULKAN_STRUCT(subpass);
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = colorRenferences.size();
    subpass.pColorAttachments = colorRenferences.data();
    subpass.pResolveAttachments = resolveReferences.data();

    auto &inputReference = inputReferences.emplace_back();
    inputReference.attachment = depthReference.attachment;
    inputReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    subpass.inputAttachmentCount = 1;
    subpass.pInputAttachments = &inputReference;

    auto &subpassDep = subpassDependency.emplace_back();
    subpassDep.srcSubpass = 0;
    subpassDep.dstSubpass = 1;
    subpassDep.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                              VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    subpassDep.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    subpassDep.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    subpassDep.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    subpassDep.dependencyFlags = 0;
    // subpassDep.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
  }

  // Two subpasses for deferred shading
  if (initializer.subpassType == SubpassType::DeferredShading) {
    // both sub-passes only test DepthStencil
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    // 1. Write to SceneColor and GBuffer, input Depth
    {
      auto &subpass = subpassDescriptions.emplace_back();
      ZERO_VULKAN_STRUCT(subpass);
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = colorRenferences.size();
      subpass.pColorAttachments = colorRenferences.data();
      subpass.pDepthStencilAttachment = &depthReference;
      subpass.inputAttachmentCount = 1;
      subpass.pInputAttachments = &depthReference;

      auto &subpassDep = subpassDependency.emplace_back();
      subpassDep.srcSubpass = 0;
      subpassDep.dstSubpass = 1;
      subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                                VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                                VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      subpassDep.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      subpassDep.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                 VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      subpassDep.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
      subpassDep.dependencyFlags = 0;
    }

    // 2. Write to SceneColor, input GBuffer and DepthStencil
    {
      auto &subpass = subpassDescriptions.emplace_back();
      ZERO_VULKAN_STRUCT(subpass);
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = colorRenferences.data();  // SceneColor only
      subpass.pDepthStencilAttachment = &depthReference;

      // Depth as Input0
      {
        auto &inputReference = inputReferences.emplace_back();
        inputReference.attachment = depthReference.attachment;
        inputReference.layout = depthReference.layout;
      }

      // Placeholder for SceneColor Attachment but unused
      {
        auto &inputReference = inputReferences.emplace_back();
        inputReference.attachment = VK_ATTACHMENT_UNUSED;
        inputReference.layout = VK_IMAGE_LAYOUT_UNDEFINED;
      }

      // GBufferA/B/C/D as Input2/3/4/5
      {
        int numColorRefs = colorRenferences.size();
        for (int i = 1; i < numColorRefs; ++i) {
          auto &inputReference = inputReferences.emplace_back();
          inputReference.attachment = colorRenferences[i].attachment;
          inputReference.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
      }

      subpass.inputAttachmentCount = inputReferences.size();
      subpass.pInputAttachments = inputReferences.data();

      auto &subpassDep = subpassDependency.emplace_back();
      subpassDep.srcSubpass = 1;
      subpassDep.dstSubpass = 2;
      subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      subpassDep.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      subpassDep.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      subpassDep.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
      subpassDep.dependencyFlags = 0;
    }
  }

  ZERO_VULKAN_STRUCT(createInfo);
  createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  createInfo.attachmentCount = attachments.size();
  createInfo.pAttachments = attachments.data();
  createInfo.subpassCount = subpassDescriptions.size();
  createInfo.pSubpasses = subpassDescriptions.data();
  createInfo.dependencyCount = subpassDependency.size();
  createInfo.pDependencies = subpassDependency.data();

  if (vkCreateRenderPass(m_device->getDevice(), &createInfo, nullptr,
                         &m_renderPass) != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "vkCreateRenderPass failed!");
  }
}

std::shared_ptr<VulkanRenderPass> VulkanRenerPassManager::getOrCreateRenderPass(
    const RHIGraphicsPipelineStateInitiazlier &initializer) {
  uint32_t renderpassHash = initializer.getRenderPassHash();
  if (auto it = m_renderPasses.find(renderpassHash);
      it != m_renderPasses.end()) {
    return it->second;
  } else {
    std::shared_ptr<VulkanRenderPass> renderpass =
        std::make_shared<VulkanRenderPass>(m_device, initializer);
    m_renderPasses[renderpassHash] = renderpass;
    return renderpass;
  }
}

}  // namespace GEngine