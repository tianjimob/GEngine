#include "rotator.h"

#include <cmath>

#include "core/math/math.h"
#include "core/math/matrix4.h"

namespace GEngine {

Matrix4x4 Rotator::getInverseRotatorMatrix() const {
  float cp = std::cos(Degree{pitch}.valueRadians());
  float sp = std::sin(Degree{pitch}.valueRadians());
  float cy = std::cos(Degree{yaw}.valueRadians());
  float sy = std::sin(Degree{yaw}.valueRadians());
  float cr = std::cos(Degree{roll}.valueRadians());
  float sr = std::sin(Degree{roll}.valueRadians());
  return Matrix4x4{
             {1, 0, 0, 0}, {0, cp, -sp, 0}, {0, sp, cp, 0}, {0, 0, 0, 1}} *
         Matrix4x4{
             {cy, 0, sy, 0}, {0, 1, 0, 0}, {-sy, 0, cy, 0}, {0, 0, 0, 1}} *
         Matrix4x4{{cr, -sr, 0, 0}, {sr, cr, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
}

}  // namespace GEngine