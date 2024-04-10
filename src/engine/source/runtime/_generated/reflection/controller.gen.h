#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\actor\controller\controller.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterController {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Controller"};
    classBuilder

        .setSuperClassName("GEngine::Actor")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
