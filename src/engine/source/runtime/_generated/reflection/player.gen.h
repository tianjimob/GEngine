#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\player\player.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterPlayer {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Player"};
    classBuilder

        .setSuperClassName("GEngine::GObject")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
