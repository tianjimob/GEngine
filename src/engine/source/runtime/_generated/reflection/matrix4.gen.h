#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\matrix4.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterMatrix4x4_ {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Matrix4x4_"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
