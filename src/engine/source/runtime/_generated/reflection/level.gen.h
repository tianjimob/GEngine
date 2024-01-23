#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\level\level.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterLevel {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Level"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
