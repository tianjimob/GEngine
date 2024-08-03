#pragma once

#include "function/framework/render/core/shaders.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace GEngine {

class VulkanRHIComputePipelineState;

class RHIBuffer {
public:
  RHIBuffer(int size) : m_size(size) {}
  virtual ~RHIBuffer() = default;
  int size() const { return m_size; }
  virtual void* getHandle() = 0;
private:
  int m_size;
};

class RHIUniformBuffer {};

class RHIShader {
public:
  virtual ~RHIShader() = default;
  
  const std::string &getShaderName() const { return m_shaderName; }
  void setShaderName(const std::string &shaderName) {
    m_shaderName = shaderName;
  }

  std::vector<ShaderPrameterMember> &getMembers() { return m_members; }
  void setMembers(const std::vector<ShaderPrameterMember> &members) {
    m_members = members;
  }

private:
  std::vector<ShaderPrameterMember> m_members;

private:
  std::string m_shaderName = "Empty";
};

class RHIGraphicsShader : public RHIShader {};

class RHIComputeShader : public RHIShader {};

struct RHIComputePipelineState {};
class RHICommandContext {
public:
  virtual void RHIDispatchComputeShader(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
  virtual void RHISetComputePipelineState(std::shared_ptr<RHIComputePipelineState>& computePipelineState, const void* parametersData) = 0;
};

class RHICommandBufferImmediate {};

class RHICommandBufferExecutor {
 public:
  RHICommandBufferImmediate &getCommandBufferImmediate() {
    return m_commandBufferImmediate;
  }

private:
  RHICommandBufferImmediate m_commandBufferImmediate;
};

class RHIImage {};

struct RHIGraphicsPipelineState {};

using RHIGraphicsPipelineStateRef = std::shared_ptr<RHIGraphicsPipelineState>;

class RHIGraphicsPipelineStateCreateInfo {
 public:
 private:
};



}  // namespace GEngine