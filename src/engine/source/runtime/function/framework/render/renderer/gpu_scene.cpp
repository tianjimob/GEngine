#include "gpu_scene.h"

#include <algorithm>
#include <execution>
#include <memory>

#include "core/math/math.h"
#include "function/framework/render/core/shaders.h"
#include "function/framework/render/renderer/upload_buffer.h"
#include "function/framework/render/rhi/compute_shader_utils.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/render/rhi/rhi_shader.h"
#include "function/framework/render/rhi/rhi_type.h"
#include "scene.h"
#include "shaders/scene_data.h"


namespace GEngine {

void GPUScene::update(std::shared_ptr<Scene> &scene) {
  updateBufferState(scene);

  // upload
  {
    UploadBuffer primitiveUploader;

    primitiveUploader.begin(bufferState.primitiveBuffer, m_primitivesToUpdate.size(), sizeof(PrimitiveSceneData::data));

    // prepare data
    {
      std::for_each(std::execution::par, m_primitivesToUpdate.begin(),
                    m_primitivesToUpdate.end(),
                    [&scene, &primitiveUploader](uint32_t index) {
                      const auto *primitiveSceneProxy =
                          scene->primitiveSceneProxies[index];
                      PrimitiveSceneData sceneData{primitiveSceneProxy};
                      Vector4 *dstData = reinterpret_cast<Vector4 *>(
                          primitiveUploader.getMappedVector4(index));
                      for (int i = 0;
                           i < PrimitiveSceneData::DataStrideInFloat4s; ++i) {
                        dstData[i] = sceneData.data[i];
                      }
                    });
    }
  }
}
void GPUScene::updateBufferState(std::shared_ptr<Scene> &scene) {
  constexpr int32_t initialBufferSize = 256;

  const uint32_t primitiveSizeReserve = roundUpToPowerOfTwo(
      std::max((int32_t)scene->primitives.size(), initialBufferSize) *
      sizeof(PrimitiveSceneData::data));

  auto memcpyBuffer = [](MemcpyCS::Parameters &params) {
    std::shared_ptr<RHIComputeShader> memcpyCS =
        std::static_pointer_cast<RHIComputeShader>(
            RHIShaders::getShader("MemcpyCS"));

    ComputeShaderUtils::dispatch(
        memcpyCS, params, (params.memcpyInfo.sizeInVec4 + (64 - 1)) / 64, 1, 1);
  };

  if (bufferState.primitiveBuffer == nullptr ||
      bufferState.primitiveBuffer->size() != primitiveSizeReserve) {
    std::shared_ptr<RHIBuffer> newPrimitiveBuffer = GlobalRHI->createBuffer(
        primitiveSizeReserve, RHIBufferUsageFlags::StorageBuffer,
        RHIMemoryPropertyFlags::DeviceLocal);

    MemcpyCS::Parameters params;
    params.memcpyInfo.sizeInVec4 = primitiveSizeReserve / sizeof(Vector4);
    params.memcpyInfo.srcOffset = 0;
    params.memcpyInfo.dstOffset = 0;
    params.ssboSrc = bufferState.primitiveBuffer.get();
    params.ssboDst = newPrimitiveBuffer.get();
    memcpyBuffer(params);

    bufferState.primitiveBuffer = newPrimitiveBuffer;
  }
}

}  // namespace GEngine