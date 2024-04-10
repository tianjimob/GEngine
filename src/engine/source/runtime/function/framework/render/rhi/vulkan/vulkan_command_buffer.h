#pragma once

#include "vulkan/vulkan.h"
#include <memory>
#include <vector>

namespace GEngine {

class VulkanDevice;
class VulkanCommandBuffer;
class VulkanRHICommandContext;
class VulkanQueue;
class VulkanSemaphore;
class VulkanFence;

class VulkanCommandPool {
public:
  VulkanCommandPool(VulkanDevice *device);
  ~VulkanCommandPool();

  VkCommandPool getCommandPool() { return m_commandPool; }
  std::vector<std::shared_ptr<VulkanCommandBuffer>>& getCommandBuffers() { return m_commandBuffers; }

  void init(uint32_t queueFamilyIndex);
  std::shared_ptr<VulkanCommandBuffer>& create();

private:
  VkCommandPool m_commandPool;
  VulkanDevice *m_device;

  std::vector<std::shared_ptr<VulkanCommandBuffer>> m_commandBuffers;
};

class VulkanCommandBuffer {
public:
  VulkanCommandBuffer(VulkanDevice *device, VulkanCommandPool *commandPool);

  VkCommandBuffer getCommandBuffer() { return m_commandBuffer; }
  std::shared_ptr<VulkanFence>& getFence() { return m_fence; }

  void begin();
  void end();

  void endRenderPass();

  bool needReset() { return m_state == State::NeedReset; }
  bool isInitial() { return m_state == State::Initial; }
  bool isInsideBegin() { return m_state == State::InsideBegin; }
  bool isInsideRenderPass() { return m_state == State::InsideRenderPass; }
  bool isRecording() {
    return m_state == State::InsideBegin || m_state == State::InsideRenderPass;
  }

  enum class State {
    NotAllocate,
    Initial,
    InsideBegin,      // recording
    InsideRenderPass, // recording
    Executable,
    Pending,
    NeedReset
  };

  void setState(State state) { m_state = state; }

  void refreshFenceStatus();

private:
  VulkanDevice* m_device;
  VulkanCommandPool *m_commandPool;
  State m_state;
  VkCommandBuffer m_commandBuffer;

  std::shared_ptr<VulkanFence> m_fence;

  void alloc();
};

class VulkanCommandBufferManager {
public:
  VulkanCommandBufferManager(VulkanDevice *device,
                             VulkanRHICommandContext *context);
  void init();

  void submitActiveCommandBuffer(
      std::vector<std::shared_ptr<VulkanSemaphore>> signalSemaphores);
  void prepareForNewActiveCommandBuffer();
  
private:
  VulkanDevice *m_device;
  VulkanRHICommandContext *m_context;
  VulkanCommandPool m_pool;
  VulkanQueue *m_queue;
  std::shared_ptr<VulkanCommandBuffer> m_activeCommandBuffer;
};

}