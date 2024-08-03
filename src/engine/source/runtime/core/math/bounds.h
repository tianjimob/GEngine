#pragma once

#include "core/math/math_sse.h"
#include "core/math/vector4.h"

#include <cfloat>

namespace GEngine {

struct Bounds {
  // only need min.xyz or max.xyz, using Vector4 instead of Vector3 for SSE
  Vector4 min {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
  Vector4 max {-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};

  Bounds &operator+=(const Bounds &other) {
    simdStore((float *)&min, simdMin(simdLoad((float *)&min),
                                           simdLoad((float *)&other.min)));
    simdStore((float *)&max, simdMax(simdLoad((float *)&max),
                                           simdLoad((float *)&other.max)));
    return *this;
  }

  Bounds operator+(const Bounds &other) const{
    return Bounds{*this} += other;
  }

};

}