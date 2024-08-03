#pragma once

#include <cstddef>
#include <cstdint>
namespace GEngine {

class RHIBuffer;

template <typename T> struct ShaderTypeInfo {};

template <> struct ShaderTypeInfo<uint32_t> {
  static constexpr uint32_t Elements = 0;
};

template <> struct ShaderTypeInfo<RHIBuffer*> {
  static constexpr uint32_t Elements = 0;
};

template <typename T, size_t size> struct ShaderTypeInfo<T[size]> {
  static constexpr uint32_t Elements = size;
};

struct MemcpyInfo {
  uint32_t sizeInVec4;
  uint32_t srcOffset;
  uint32_t dstOffset;
};

template <> struct ShaderTypeInfo<MemcpyInfo> {
  static constexpr uint32_t Elements = 0;
};

}