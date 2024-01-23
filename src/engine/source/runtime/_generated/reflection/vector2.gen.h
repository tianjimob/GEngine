#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\vector2.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterVector2 {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Vector2"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
