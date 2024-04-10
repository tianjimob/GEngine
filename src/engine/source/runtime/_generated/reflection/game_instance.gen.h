#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\game_instance\game_instance.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterGameInstance {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::GameInstance"};
    classBuilder

        .setSuperClassName("GEngine::GObject")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
