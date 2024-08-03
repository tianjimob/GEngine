#include "catch_amalgamated.hpp"
#include "core/math/bounds.h"
#include "core/math/vector4.h"

TEST_CASE("Bounds", "[Bounds]") {
  GEngine::Bounds a;
  a.min = {1, 2, 3, 0};
  a.max = {4, 5, 6, 0};

  GEngine::Bounds b;
  b.min = {10, 20, 30, 0};
  b.max = {40, 50, 60, 0};

  GEngine::Bounds c = a + b;

  REQUIRE(c.min == GEngine::Vector4{1, 2, 3, 0});
  REQUIRE(c.max == GEngine::Vector4{40, 50, 60, 0});

  BENCHMARK("Bounds Time") {
    for (int i = 0; i < 5000000; ++i) {
      GEngine::Bounds c = a + b;
    }
  };
}