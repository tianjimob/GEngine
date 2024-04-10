#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\component\component.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterActorComponent {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::ActorComponent"};
    classBuilder

        .setSuperClassName("GEngine::GObject")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
