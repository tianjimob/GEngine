#pragma once

#include "rhi_resource.h"
#include <memory>
#include <string_view>

namespace GEngine {

class RHI {
public:
  virtual void init() = 0;
  virtual std::string_view getName() = 0;
  
  virtual RHIGraphicsPipelineStateRef createGraphicsPipelineState(const RHIGraphicsPipelineStateCreateInfo& createInfo) = 0;
};

extern RHI *GlobalRHI;

}