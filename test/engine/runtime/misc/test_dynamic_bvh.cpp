#include <cstdlib>
#include <random>
#include <vector>

#include "catch_amalgamated.hpp"
#include "core/math/bounds.h"
#include "core/math/vector4.h"
#include "function/framework/render/renderer/dynamic_bvh.h"

TEST_CASE("DynamicBVH", "[DynamicBVH]") {
  GEngine::DynamicBVH<4> bvh;
  std::vector<GEngine::Bounds> boundsArray;
  boundsArray.resize(65536);

  // 创建一个随机设备
  std::random_device rd;

  // 使用随机设备生成种子
  std::mt19937 gen(rd());

  // 定义一个均匀分布在0到1之间的浮点数分布
  std::uniform_real_distribution<float> dis(0.0, 1.0);

  for (int i = 0; i < 65536; ++i) {
    GEngine::Vector4 center{-64 + 128 * dis(gen), -64 + 128 * dis(gen),
                            -64 + 128 * dis(gen), -64 + 128 * dis(gen)};
    GEngine::Vector4 extent{1 + 3 * dis(gen), 1 + 3 * dis(gen),
                            1 + 3 * dis(gen), 1 + 3 * dis(gen)};
    boundsArray[i].min = center - extent;
    boundsArray[i].max = center + extent;
  }
  BENCHMARK("DynamicBVH Time") {
    for (int i = 0; i < 65536; ++i) {
      bvh.add(boundsArray[i], i);
    }
  };

  REQUIRE(bvh.check() == true);
}