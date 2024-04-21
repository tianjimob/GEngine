#pragma once

#include "core/math/vector3.h"
#include "input_types.h"
#include <cstring>

namespace GEngine {

struct KeyState {
  // This is the most recent raw value reported by the device.  For digital buttons, 0 or 1.  For analog buttons, 0->1.  For axes, -1->1. The X field is for non-vector keys
  Vector3 rawValue;

  // processed rawValue
  Vector3 value;

  // True if this key has been "consumed" by an InputComponent and should be ignored for further components during this update.
  bool consumed;

  uint32_t count[static_cast<int>(InputEvent::Num)];

  KeyState() : consumed(false) { memset(count, 0, sizeof(count)); }
};

}