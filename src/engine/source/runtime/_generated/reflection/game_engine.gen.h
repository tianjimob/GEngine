#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\engine\game_engine\game_engine.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterGameEngine {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::GameEngine"};
    classBuilder

        .setSuperClassName("GEngine::Engine")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
