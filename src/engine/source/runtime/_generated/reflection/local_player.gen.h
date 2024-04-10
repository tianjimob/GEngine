#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\player\local_player\local_player.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterLocalPlayer {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::LocalPlayer"};
    classBuilder

        .setSuperClassName("GEngine::Player")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
