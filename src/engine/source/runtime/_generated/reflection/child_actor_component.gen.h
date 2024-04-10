#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\component\child_actor\child_actor_component.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterChildActorComponent {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::ChildActorComponent"};
    classBuilder

        .setSuperClassName("GEngine::TransformComponent")

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
