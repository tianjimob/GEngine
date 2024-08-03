#pragma once

#include "core/math/box_sphere_bounds.h"
#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include <vector>

namespace GEngine {

struct RawVertexBuffer {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> tangents;
  std::vector<float> uvs;
};

struct RawIndexBuffer {
  std::vector<uint32_t> indices;
};

struct RawDataBuffer {
  RawVertexBuffer vertexBuffer;
  RawIndexBuffer indexBuffer;
};

struct StaticMesh {
  RawDataBuffer rawData;
  BoxSphereBounds bounds;
};

}