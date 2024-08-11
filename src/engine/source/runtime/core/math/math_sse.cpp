#include "math_sse.h"

namespace GEngine {

void simdMatrix4Multiply(Matrix4x4 *result, const Matrix4x4 *left,
                  const Matrix4x4 *right) {
  const SimdVectorFloat4 *a = reinterpret_cast<const SimdVectorFloat4 *>(left);
  const SimdVectorFloat4 *b = reinterpret_cast<const SimdVectorFloat4 *>(right);
  SimdVectorFloat4 *r = reinterpret_cast<SimdVectorFloat4 *>(result);
  SimdVectorFloat4 temp, r0, r1, r2;

  // left[0] * right
  temp = simdMultiply(simdReplicate<0>(a[0]), b[0]);
  temp = simdMultiplyAdd(simdReplicate<1>(a[0]), b[1], temp);
  temp = simdMultiplyAdd(simdReplicate<2>(a[0]), b[2], temp);
  r0 = simdMultiplyAdd(simdReplicate<3>(a[0]), b[3], temp);

  // left[1] * right
  temp = simdMultiply(simdReplicate<0>(a[1]), b[0]);
  temp = simdMultiplyAdd(simdReplicate<1>(a[1]), b[1], temp);
  temp = simdMultiplyAdd(simdReplicate<2>(a[1]), b[2], temp);
  r1 = simdMultiplyAdd(simdReplicate<3>(a[1]), b[3], temp);

  // left[2] * right
  temp = simdMultiply(simdReplicate<0>(a[2]), b[0]);
  temp = simdMultiplyAdd(simdReplicate<1>(a[2]), b[1], temp);
  temp = simdMultiplyAdd(simdReplicate<2>(a[2]), b[2], temp);
  r2 = simdMultiplyAdd(simdReplicate<3>(a[2]), b[3], temp);

  // left[3] * right
  temp = simdMultiply(simdReplicate<0>(a[3]), b[0]);
  temp = simdMultiplyAdd(simdReplicate<1>(a[3]), b[1], temp);
  temp = simdMultiplyAdd(simdReplicate<2>(a[3]), b[2], temp);
  temp = simdMultiplyAdd(simdReplicate<3>(a[3]), b[3], temp);

  r[0] = r0;
  r[1] = r1;
  r[2] = r2;
  r[3] = temp;
}

}