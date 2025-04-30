#include "catch_amalgamated.hpp"
#include "core/math/bounds.h"
#include "core/math/vector4.h"

inline GEngine::Bounds BoundPlus(const GEngine::Bounds &lhs, const GEngine::Bounds &rhs) { 
  GEngine::Bounds bound;
  bound.min = {std::min(lhs.min.x, rhs.min.x), std::min(lhs.min.y, rhs.min.y), std::min(lhs.min.z, rhs.min.z), std::min(lhs.min.w, rhs.min.w)};
  bound.max = {std::max(lhs.max.x, rhs.max.x), std::max(lhs.max.y, rhs.max.y), std::max(lhs.max.z, rhs.max.z), std::max(lhs.max.w, rhs.max.w)};
  return bound;
}

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

  BENCHMARK("Bounds Time SIMD") {
    for (int i = 0; i < 1000000; ++i) {
      GEngine::Bounds c = a + b;
      Catch::Benchmark::deoptimize_value(c);
    }
  };

  BENCHMARK("Bounds Time Non SIMD") {
    for (int i = 0; i < 1000000; ++i) {
      GEngine::Bounds c = BoundPlus(a, b);
      Catch::Benchmark::deoptimize_value(c);
    }
  };
}