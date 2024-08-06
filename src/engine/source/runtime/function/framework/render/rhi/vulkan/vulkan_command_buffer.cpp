#include "vulkan_command_buffer.h"

#include <cassert>
#include <memory>
#include <vector>

#include "core/log/logger.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_context.h"
#include "vulkan_device.h"
#include "vulkan_queue.h"
#include "vulkan_sync.h"

namespace GEngine {

static DECLARE_LOG_CATEGORY(LogVulkanRHI);

VulkanCommandPool::VulkanCommandPool(VulkanDevice *device)
    : m_commandPool(nullptr), m_device(device) {}

VulkanCommandPool::~VulkanCommandPool() {
  vkDestroyCommandPool(m_device->getDevice(), m_commandPool, nullptr);
}

void VulkanCommandPool::init(uint32_t queueFamilyIndex) {
  VkCommandPoolCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.queueFamilyIndex = queueFamilyIndex;
  createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  auto result = vkCreateCommandPool(m_device->getDevice(), &createInfo, nullptr,
                                    &m_commandPool);
  if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkCreateCommandPool", result, __FILE__, __LINE__);
  }
}

std::shared_ptr<VulkanCommandBuffer>& VulkanCommandPool::create() {
  return m_commandBuffers.emplace_back(
      std::make_shared<VulkanCommandBuffer>(m_device, this));
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device,
                                         VulkanCommandPool* commandPool)
    : m_device(device), m_commandPool(commandPool), m_state(State::NotAllocate) {
  alloc();
  m_fence = std::make_shared<VulkanFence>(*m_device);
}

void VulkanCommandBuffer::alloc() {
  VkCommandBufferAllocateInfo allocInfo;
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.pNext = nullptr;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;
  allocInfo.commandPool = m_commandPool->getCommandPool();
  auto result = vkAllocateCommandBuffers(m_device->getDevice(), &allocInfo,
                                         &m_commandBuffer);
  if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkAllocateCommandBuffers", result, __FILE__, __LINE__);
  }
  m_state = State::Initial;
}

void VulkanCommandBuffer::begin() {
  if (m_state == State::NeedReset)
    vkResetCommandBuffer(m_commandBuffer,
                         VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
  else
    assert(m_state == State::Initial);
  m_state = State::InsideBegin;

  VkCommandBufferBeginInfo beginInfo;
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = nullptr;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  beginInfo.pInheritanceInfo = nullptr;

  auto result = vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
  if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkBeginCommandBuffer", result, __FILE__, __LINE__);
  }
}

void VulkanCommandBuffer::end() {
  assert(isInsideBegin() && "Can't End as we're inside a render pass!");
  auto result = vkEndCommandBuffer(m_commandBuffer);
  if (result != VK_SUCCESS) {
    LOG_FATAL(LogVulkanRHI, "{} failed, VkResult={} at {}:{}",
              "vkEndCommandBuffer", result, __FILE__, __LINE__);
  }
}

void VulkanCommandBuffer::endRenderPass()
{
  assert(isInsideRenderPass() && "Can't EndRP as we're NOT inside one!");
  vkCmdEndRenderPass(m_commandBuffer);
  m_state = State::InsideBegin;
}

void VulkanCommandBuffer::refreshFenceStatus()
{
  if (m_state == State::Pending) {
    if (m_fence->isSingaled()) {
      m_fence->reset();
      m_state = State::NeedReset;
    }
  }
}

VulkanCommandBufferManager::VulkanCommandBufferManager(
    VulkanDevice* device, VulkanRHICommandContext* context)
    : m_device(device),
      m_context(context),
      m_pool(device),
      m_queue(context->getQueue()) {
  m_pool.init(m_queue->getFamilyIndex());
  m_activeCommandBuffer = m_pool.create();
}

void VulkanCommandBufferManager::init() {
  m_activeCommandBuffer->begin();
}

void VulkanCommandBufferManager::submitActiveCommandBuffer(std::vector<std::shared_ptr<VulkanSemaphore>> signalSemaphores)
{
  assert(m_activeCommandBuffer);

  std::vector<VkSemaphore> semaphores;
  semaphores.reserve(signalSemaphores.size());
  for (auto& signalSemaphore :
       signalSemaphores) {
    semaphores.emplace_back(signalSemaphore->getSemaphore());
  }

  if (m_activeCommandBuffer->isRecording()) {
    if (m_activeCommandBuffer->isInsideRenderPass()) {
      LOG_WARN(LogVulkanRHI, "Forcing EndRenderPass() for submission");
      m_activeCommandBuffer->endRenderPass();
    }

    m_activeCommandBuffer->end();

    m_queue->submit(*m_activeCommandBuffer, semaphores.size(),
                    semaphores.data());
  }

  m_activeCommandBuffer.reset();
}

void VulkanCommandBufferManager::prepareForNewActiveCommandBuffer()
{
  auto &commandBuffers = m_pool.getCommandBuffers();
  for (auto &commandBuffer : commandBuffers) {
    commandBuffer->refreshFenceStatus();
    if (commandBuffer->isInitial() || commandBuffer->needReset()) {
      m_activeCommandBuffer = commandBuffer;
      m_activeCommandBuffer->begin();
      return;
    }
  }
  m_activeCommandBuffer = m_pool.create();
  m_activeCommandBuffer->begin();
}

}  // namespace GEngine