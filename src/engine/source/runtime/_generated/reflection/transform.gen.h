#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\transform.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterTransform {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Transform"};
    classBuilder

        .setSuperClassName("GEngine::GObject")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
