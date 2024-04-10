#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\engine\game_viewport_client.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterGameViewportClient {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::GameViewportClient"};
    classBuilder

        .setSuperClassName("GEngine::GObject")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
