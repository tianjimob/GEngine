#pragma once

#include "core/container/static_array.h"
#include "core/math/vector4.h"
#include <array>
#include <cstdint>

namespace GEngine {

class PrimitiveSceneProxy;
struct PrimitiveSceneData {
  static constexpr int DataStrideInFloat4s = 13;
  StaticArray<Vector4, DataStrideInFloat4s> data;
  PrimitiveSceneData(const PrimitiveSceneProxy* primitiveSceneProxy, uint32_t primitiveId);
};

struct PrimitiveInstanceSceneData {
  static constexpr int DataStrideInFloat4s = 10;
  StaticArray<Vector4, DataStrideInFloat4s> data;
  PrimitiveInstanceSceneData(const PrimitiveSceneProxy* primitiveSceneProxy,
                             uint32_t primitiveId, uint32_t instanceIndex);
};

}