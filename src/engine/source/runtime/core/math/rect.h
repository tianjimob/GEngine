#pragma once

#include <cstdint>

#include "core/math/ivector2.h"
#include "core/math/vector2.h"

namespace GEngine {

struct Rect {
  IVector2 min;  // left up
  IVector2 max;  // right down

  int32_t width() const { return max.x - min.x; }
  int32_t height() const { return max.y - min.y; }
};

}  // namespace GEngine