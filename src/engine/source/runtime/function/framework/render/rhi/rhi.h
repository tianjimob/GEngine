#pragma once

#include "function/framework/render/rhi/rhi_type.h"
#include "rhi_resource.h"
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace GEngine {

struct RHIRasterizerStateInitializer {
  CullMode cullMode;
  PolygonMode polgonMode;
};

struct RHIDepthStencilStateInitializer {
  bool enableDepthTest;
  CompareOperator depthCompareOp;
  bool enableDepthWrite;
  bool enableStencilTest;
  StencilOperator frontFailOp;
  StencilOperator frontPassOp;
  StencilOperator frontDepthFailOp;
  CompareOperator frontCompareOp;
  StencilOperator backFailOp;
  StencilOperator backPassOp;
  StencilOperator backDepthFailOp;
  CompareOperator backCompareOp;
};

struct RHIBlendStateInitializer {
  struct BlendState {
    bool           blendEnable;
    BlendFactor    srcColorBlendFactor;
    BlendFactor    dstColorBlendFactor;
    BlendOperator  colorBlendOp;
    BlendFactor    srcAlphaBlendFactor;
    BlendFactor    dstAlphaBlendFactor;
    BlendOperator  alphaBlendOp;
    ColorWriteMask colorWriteMask;
  };
  std::vector<BlendState> blendStates;
};

class RHI {
public:
  virtual void init() = 0;
  virtual std::string_view getName() = 0;

  virtual std::shared_ptr<RHIGraphicsPipelineState> createGraphicsPipelineState(
      const RHIGraphicsPipelineStateInitiazlier &createInfo) = 0;

  virtual std::shared_ptr<RHIComputePipelineState> createComputePipelineState(std::shared_ptr<RHIComputeShader>& computeShader) = 0;

  virtual std::shared_ptr<RHIUniformBuffer> createUniformBuffer(uint32_t size) = 0;

  virtual std::shared_ptr<RHIBuffer>
  createBuffer(uint32_t size, RHIBufferUsageFlags usage,
               RHIMemoryPropertyFlags property) = 0;

  virtual std::shared_ptr<RHIBuffer> createVertexBuffer(const void *data,
                                                        uint32_t size) = 0;

  virtual std::shared_ptr<RHIBuffer> createIndexBuffer(const void *data, uint32_t size) = 0;

  virtual std::shared_ptr<RHIComputeShader>
  createComputeShader(const std::vector<uint8_t> &shaderCode) = 0;

  virtual std::shared_ptr<RHIRasterizerState>
  createRasterizerState(const RHIRasterizerStateInitializer &initializer) = 0;

  virtual std::shared_ptr<RHIDepthStencilState>
  createDepthStencilweState(const RHIDepthStencilStateInitializer &initializer) = 0;

  virtual std::shared_ptr<RHIBlendState>
  createBlendState(const RHIBlendStateInitializer &initializer) = 0;

  virtual void mapBuffer(void *data, std::shared_ptr<RHIBuffer> &buffer) = 0;

  virtual void unmapBuffer(std::shared_ptr<RHIBuffer> &buffer) = 0;

  virtual std::shared_ptr<RHICommandContext> getComputeContext() = 0;
};

extern RHI *GlobalRHI;

}