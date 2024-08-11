#pragma once

#include "core/math/matrix4.h"
namespace GEngine {

/*
  The vulkan coordinate system is used for the world coordinate system, and the
  z axis is toward the outside of the screen. The positive direction of rotation
  is defined as the counterclockwise direction along the coordinate axis.

  pitch, yaw and roll (store in degress) are the rotation angles around the x axis, the y axis and
  the z axis respectively.
  
  The rotation matrix of the Rotator is R = R(roll) * R(yaw) * R(pitch).
*/
struct Rotator {
  float pitch;
  float yaw;
  float roll;

public:
  // Inv(R) = R(-pitch) * R(-yaw) * R(-roll)
  Matrix4x4 getInverseRotatorMatrix() const;
};

}