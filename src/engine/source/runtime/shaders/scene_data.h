#pragma once

#include "core/container/static_array.h"
#include "core/math/vector4.h"
#include <array>

namespace GEngine {

struct PrimitiveSceneData {
  static constexpr int DataStrideInFloat4s = 9;
  StaticArray<Vector4, DataStrideInFloat4s> data;
};

}