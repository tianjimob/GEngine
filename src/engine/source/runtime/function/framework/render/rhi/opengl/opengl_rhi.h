#pragma once

#include "function/framework/render/rhi/rhi.h"
#include "glad/glad.h"

namespace GEngine {

class OpenGLRHI : public RHI {
  static inline OpenGLRHI* instance { nullptr };
public:
  OpenGLRHI();

  virtual std::string_view getName() override { return "OpenGL45"; }

private:
  void createContext();
};

}