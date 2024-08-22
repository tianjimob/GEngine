#pragma once

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/mesh_pipeline/vertex_streams.h"
#include "function/framework/render/rhi/rhi_type.h"
#include <cstddef>
#include <cstdint>
#include <functional>
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

class RHIVertexShader : public RHIShader {};

class RHIPixelShader : public RHIShader {};

class RHIComputeShader : public RHIShader {};

struct RHIComputePipelineState {};

struct RHIGraphicsPipelineState {};

struct RHIViewport {};
class RHICommandContext {
public:
  virtual void RHIDispatchComputeShader(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
  virtual void RHISetComputePipelineState(
      std::shared_ptr<RHIComputePipelineState> &computePipelineState,
      const void *parametersData) = 0;
  virtual void RHISetGraphicsPipelineState(
      std::shared_ptr<RHIGraphicsPipelineState> &graphicsPipelineState,
      const void *parametersData) = 0;
  virtual void RHICopyBuffer(std::shared_ptr<RHIBuffer> &srcBuffer,
                             std::shared_ptr<RHIBuffer> &dstBuffer) = 0;
  virtual void
  RHIDrawIndexedPrimitive(std::shared_ptr<RHIBuffer> &IndexBufferRHI,
                          int32_t BaseVertexIndex, uint32_t FirstInstance,
                          uint32_t NumVertices, uint32_t StartIndex,
                          uint32_t NumPrimitives, uint32_t NumInstances) = 0;
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

struct RHIBlendState {};

struct RHIRasterizerState {};

struct RHIDepthStencilState {};

struct PipelineShaderState {
  std::shared_ptr<RHIVertexShader> vertexShader;
  std::shared_ptr<RHIPixelShader> pixelShader;
};

class RHIGraphicsPipelineStateInitiazlier {
public:
  PipelineShaderState shaderState;
  VertexStreams vertexStreams;
  std::shared_ptr<RHIRasterizerState> rasterizerState;
  std::shared_ptr<RHIDepthStencilState> depthStencilState;
  std::shared_ptr<RHIBlendState> blendState;

  RHIPrimitiveType primitiveType;
  uint16_t numSamples;

  // renderpass-----------------
  std::vector<RHIFormat> colorAttachmentFormats;
  bool hasDepthAttachment;
  RHIFormat depthAttachmentFormat;
  AttachmentLoadOp depthLoadOp;
  AttachmentStoreOp depthStoreOp;
  AttachmentLoadOp stencilLoadOp;
  AttachmentStoreOp stencilStoreOp;
  SubpassType subpassType;
  // renderpass------------------

  int getAttachmentsSize() const{
    if (numSamples == 1) {
      return hasDepthAttachment ? colorAttachmentFormats.size() + 1 : colorAttachmentFormats.size();
    } else {
      return hasDepthAttachment ? colorAttachmentFormats.size() * 2 + 1
                                : colorAttachmentFormats.size() * 2;
    }
  }

  uint32_t getRenderPassHash() const;

 private:
};



}  // namespace GEngine