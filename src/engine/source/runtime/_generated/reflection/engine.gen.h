#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\engine\engine.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterEngine {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Engine"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
