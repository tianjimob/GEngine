#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\actor\game_mode\game_mode.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterGameMode {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::GameMode"};
    classBuilder

        .setSuperClassName("GEngine::Actor")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
