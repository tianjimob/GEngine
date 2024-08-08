#pragma once

#include <cstdint>
namespace GEngine {

enum class RHIBufferUsageFlags : uint32_t {
  TransferSrc = 0x00000001,
  TransferDst = 0x00000002,
  UniformTexelBuffer = 0x00000004,
  StorageTexelBuffer = 0x00000008,
  UniformBuffer = 0x00000010,
  StorageBuffer = 0x00000020,
  IndexBuffer = 0x00000040,
  VertexBuffer = 0x00000080,
  IndirectBuffer = 0x00000100,
  ShaderDeviceAddress = 0x00020000,

  TransferDstAndVertexBuffer = TransferDst | VertexBuffer,
  TransferDstAndIndexBuffer = TransferDst | IndexBuffer,
};

enum class RHIMemoryPropertyFlags : uint32_t {
  DeviceLocal = 0x00000001,
  HostVisible = 0x00000002,
  HostCoherent = 0x00000004,
  HostCached = 0x00000008,
  LazilyAllocated = 0x00000010,
  Protected = 0x00000020,

  HostVisibleAndCoherent = HostVisible | HostCoherent,
};
}