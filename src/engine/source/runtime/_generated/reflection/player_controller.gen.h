#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\actor\controller\player_controller\player_controller.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterPlayerController {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::PlayerController"};
    classBuilder

        .setSuperClassName("GEngine::Controller")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
