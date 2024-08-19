#pragma once

#include <memory>

#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_resource.h"

namespace GEngine {

class ComputeShaderUtils {
public:
  template <typename ShaderParameters>
  static void dispatch(std::shared_ptr<RHIComputeShader>& computeShader,
                       const ShaderParameters& params, int groupCountX,
                       int groupCountY, int groupCountZ);
};

template <typename ShaderParameters>
void ComputeShaderUtils::dispatch(
    std::shared_ptr<RHIComputeShader>& computeShader, const ShaderParameters& params,
    int groupCountX, int groupCountY, int groupCountZ) {
  std::shared_ptr<RHIComputePipelineState> pipeline =
      GlobalRHI->createComputePipelineState(computeShader);
  std::shared_ptr<RHICommandContext> context = GlobalRHI->getComputeContext();
  context->RHISetComputePipelineState(pipeline, &params);
  context->RHIDispatchComputeShader(groupCountX, groupCountY, groupCountZ);
}
}  // namespace GEngine