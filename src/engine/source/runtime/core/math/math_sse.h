#pragma once

#include <intrin.h>
#include <xmmintrin.h>

namespace GEngine {

using SimdVectorFloat4 = __m128;

// Load 4 floats from aligned memory
inline SimdVectorFloat4 simdLoad(const float *ptr) { return _mm_load_ps(ptr); }

// Returns the minimum values of two vectors (component-wise)
inline SimdVectorFloat4 simdMin(SimdVectorFloat4 a, SimdVectorFloat4 b) {
  return _mm_min_ps(a, b);
}

// Returns the maximum values of two vectors (component-wise)
inline SimdVectorFloat4 simdMax(SimdVectorFloat4 a, SimdVectorFloat4 b) {
  return _mm_max_ps(a, b);
}

inline void simdStore(float *ptr, SimdVectorFloat4 a) {
  return _mm_store_ps(ptr, a);
}

}