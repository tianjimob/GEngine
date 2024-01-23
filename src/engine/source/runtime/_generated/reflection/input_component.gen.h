#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\component\input\input_component.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterInputComponent {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::InputComponent"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
