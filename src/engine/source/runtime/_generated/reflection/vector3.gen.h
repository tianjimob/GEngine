#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\vector3.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterVector3 {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Vector3"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
