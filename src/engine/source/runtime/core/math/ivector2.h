#pragma once

#include <cstdint>
namespace GEngine {

class alignas(8) IVector2 {
 public:
  int32_t x{0}, y{0};

  IVector2() = default;

  IVector2(int32_t x_, int32_t y_) : x(x_), y(y_) {}

  IVector2(const IVector2& other) : x(other.x), y(other.y) {}

  bool operator==(const IVector2& rhs) const {
    return (x == rhs.x && y == rhs.y);
  }

  bool operator!=(const IVector2& rhs) const {
    return (x != rhs.x || y != rhs.y);
  }

  // arithmetic operations
  IVector2 operator+(const IVector2& rhs) const {
    return IVector2(x + rhs.x, y + rhs.y);
  }

  IVector2 operator-(const IVector2& rhs) const {
    return IVector2(x - rhs.x, y - rhs.y);
  }
};

}  // namespace GEngine