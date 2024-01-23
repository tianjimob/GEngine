#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\core\math\axis_aligned.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterAxisAlignedBox {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::AxisAlignedBox"};
    classBuilder
    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
