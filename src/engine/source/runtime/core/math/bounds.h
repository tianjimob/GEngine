#pragma once

#include <cfloat>

#include "core/math/math_sse.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"


namespace GEngine {

struct Bounds {
  // only need min.xyz or max.xyz, using Vector4 instead of Vector3 for SSE
  Vector4 min{FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
  Vector4 max{-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};

  Bounds &operator+=(const Bounds &other) {
    simdStore((float *)&min, simdMin(simdLoadFloat4((float *)&min),
                                     simdLoadFloat4((float *)&other.min)));
    simdStore((float *)&max, simdMax(simdLoadFloat4((float *)&max),
                                     simdLoadFloat4((float *)&other.max)));
    return *this;
  }

  Bounds &operator+=(const Vector3 &other) {
    simdStore((float *)&min, simdMin(simdLoadFloat4((float *)&min),
                                     simdLoadFloat3((float *)&other)));
    simdStore((float *)&max, simdMax(simdLoadFloat4((float *)&max),
                                     simdLoadFloat3((float *)&other)));
    return *this;
  };

  Bounds operator+(const Bounds &other) const { return Bounds{*this} += other; }

  Vector3 getCenter() const {
    auto sum = (max + min) * 0.5;
    return {sum.x, sum.y, sum.z};
  }

  Vector3 getExtent() const {
    auto diff = (max - min) * 0.5;
    return {diff.x, diff.y, diff.z};
  }
};

}  // namespace GEngine