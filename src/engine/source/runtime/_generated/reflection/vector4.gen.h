#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\vector4.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterVector4 {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Vector4"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
