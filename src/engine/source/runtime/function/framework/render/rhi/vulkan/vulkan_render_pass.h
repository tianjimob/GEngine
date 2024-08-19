#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi_resource.h"
#include "vulkan/vulkan_core.h"
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
namespace GEngine {

class VulkanRenderPass {
public:
  VulkanRenderPass(VulkanDevice *device,
                   const RHIGraphicsPipelineStateInitiazlier &initializer);

  VkRenderPass getVkRenderPass() { return m_renderPass; }
private:
  VulkanDevice *m_device;

  // Color Attachment, Resolve Attachment, Color Attachment, Resolve Attachment, ..., Depth Attachment
  std::vector<VkAttachmentDescription> attachments;
  std::vector<VkAttachmentReference> colorRenferences;
  std::vector<VkAttachmentReference> resolveReferences;
  VkAttachmentReference depthReference;
  std::vector<VkAttachmentReference> inputReferences;

  std::vector<VkSubpassDescription> subpassDescriptions;
  std::vector<VkSubpassDependency> subpassDependency;

  VkRenderPassCreateInfo createInfo;

  VkRenderPass m_renderPass;
};

class VulkanDevice;

class VulkanRenerPassManager {
public:
 VulkanRenerPassManager(VulkanDevice* device) : m_device(device) {}
 std::shared_ptr<VulkanRenderPass> getOrCreateRenderPass(
     const RHIGraphicsPipelineStateInitiazlier& initializer);

private:
  VulkanDevice* m_device;
  std::unordered_map<uint32_t, std::shared_ptr<VulkanRenderPass>> m_renderPasses;
};
}