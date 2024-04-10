#pragma once

#include <memory>

#include "function/framework/render/rhi/rhi_resource.h"


namespace GEngine {

class VulkanDynamicRHI;
class VulkanDevice;
class VulkanQueue;
class VulkanCommandBufferManager;

class VulkanRHICommandContext : public RHICommandContext {
 public:
  VulkanRHICommandContext(VulkanDynamicRHI *rhi, VulkanDevice *device,
                          VulkanQueue *queue, bool isImmediate);
  VulkanQueue *getQueue() { return m_queue; }

 private:
  VulkanDynamicRHI *m_rhi;
  VulkanDevice *m_device;
  VulkanQueue *m_queue;
  bool m_isImmediate;

  std::shared_ptr<VulkanCommandBufferManager> m_commandBufferManager;
};

}  // namespace GEngine