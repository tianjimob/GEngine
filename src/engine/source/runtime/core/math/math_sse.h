#pragma once

#include <cstdint>
#include <emmintrin.h>
#include <excpt.h>
#include <intrin.h>
#include <xmmintrin.h>

namespace GEngine {

using SimdVectorFloat4 = __m128;
using SimdVectorInt4 = __m128i;

class Matrix4x4;

// Load 4 floats from aligned memory
inline SimdVectorFloat4 simdLoadFloat4(const float *ptr) {
  return _mm_load_ps(ptr);
}

// Loads 3 floats from unaligned memory and sets W=0.
inline SimdVectorFloat4 simdLoadFloat3(const float *ptr) {
  return _mm_setr_ps(ptr[0], ptr[1], ptr[2], 0);
}

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

inline void simdStore(int32_t *ptr, SimdVectorInt4 a) {
  return _mm_store_si128(reinterpret_cast<SimdVectorInt4*>(ptr), a);
}

inline int32_t simdTruncToInt32(float f) {
  return _mm_cvtt_ss2si(_mm_set_ss(f));
}

inline SimdVectorInt4 simdTruncToInt32(SimdVectorFloat4 f) {
  return _mm_cvttps_epi32(f);
}

template <int index>
inline SimdVectorFloat4 simdReplicate(const SimdVectorFloat4 vec) {
  return _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(index, index, index, index));
}

// SimdVectorFloat4( vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z, vec1.w+vec2.w )
inline SimdVectorFloat4 simdAdd(const SimdVectorFloat4 &vec1,
                                const SimdVectorFloat4 &vec2) {
  return _mm_add_ps(vec1, vec2);
}

// SimdVectorFloat4( vec1.x*vec2.x, vec1.y*vec2.y, vec1.z*vec2.z, vec1.w*vec2.w )
inline SimdVectorFloat4 simdMultiply(const SimdVectorFloat4 &vec1,
                                   const SimdVectorFloat4 &vec2) {
  return _mm_mul_ps(vec1, vec2);
}

// SimdVectorFloat4( a.x*b.x+c.x, a.y*b.y+c.y, a.z*b.z+c.z, a.w*b.w+c.w )
inline SimdVectorFloat4 simdMultiplyAdd(const SimdVectorFloat4 &a,
                                        const SimdVectorFloat4 &b,
                                        const SimdVectorFloat4 &c) {
  return simdAdd(simdMultiply(a, b), c);
}

void simdMatrix4Multiply(Matrix4x4 *result, const Matrix4x4 *left,
                  const Matrix4x4 *right);

}