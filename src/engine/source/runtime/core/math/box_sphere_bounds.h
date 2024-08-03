#pragma once

#include "core/math/vector3.h"
namespace GEngine {

struct BoxSphereBounds {
  Vector3 origin;
  Vector3 extent;
  float radius;
};

}