#pragma once

#include "core/math/vector3.h"
namespace GEngine {

struct BoxSphereBounds {
  Vector3 origin;
  Vector3 extent;
  float radius;

  Vector3 getBoxMin() const { return origin - extent; }

  Vector3 getBoxMax() const { return origin + extent; }
};

}