#include "gpu_scene.h"
#include "core/math/math.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/rhi_shader.h"
#include "function/framework/render/rhi/rhi_type.h"
#include "shaders/scene_data.h"
#include <algorithm>
#include <memory>

namespace GEngine {

void GPUScene::updateBufferState(std::shared_ptr<Scene> &scene) {
  constexpr int32_t initialBufferSize = 256;

  const uint32_t primitiveSizeReserve = roundUpToPowerOfTwo(
      std::max((int32_t)scene->primitives.size(), initialBufferSize) *
      sizeof(PrimitiveSceneData::data));

  auto memcpyBuffer = []() {
    auto memcpyCS = RHIShaders::getShader("MemcpyCS");
    
  };
  
  if (bufferState.primitiveBuffer == nullptr ||
      bufferState.primitiveBuffer->size() != primitiveSizeReserve) {
    std::shared_ptr<RHIBuffer> newPrimitiveBuffer = GlobalRHI->createBuffer(
        primitiveSizeReserve, RHIBufferUsageFlags::StorageBuffer,
        RHIMemoryPropertyFlags::DeviceLocal);

    

    bufferState.primitiveBuffer = newPrimitiveBuffer;
  }
}

}