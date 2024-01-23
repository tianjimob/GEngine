#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\actor\pawn\pawn.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterPawn {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Pawn"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
