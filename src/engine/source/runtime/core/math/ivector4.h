#pragma once

#include <cstdint>

namespace GEngine {

class alignas(16) IVector4 {
 public:
  int32_t x{0}, y{0}, z{0}, w{0};
};

}  // namespace GEngine
