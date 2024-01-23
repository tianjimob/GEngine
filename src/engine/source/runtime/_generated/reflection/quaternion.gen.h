#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\quaternion.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterQuaternion {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Quaternion"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
