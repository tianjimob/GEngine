#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\component\transform\transform_component.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterTransformComponent {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::TransformComponent"};
    classBuilder

        .setSuperClassName("GEngine::ActorComponent")


        .addField("m_componentToWorld", "GEngine::Transform", & ::GEngine::TransformComponent::m_componentToWorld, false)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
