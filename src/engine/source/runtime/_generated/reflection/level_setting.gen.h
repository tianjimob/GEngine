#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\actor\level_setting\level_setting.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterLevelSetting {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::LevelSetting"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
