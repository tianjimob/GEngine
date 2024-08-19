#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace GEngine {

class Scene;

struct GPUBufferState {
  std::shared_ptr<RHIBuffer> primitiveBuffer;
};

class GPUScene {
public:
  void update(std::shared_ptr<Scene> &scene);
  void updateBufferState(std::shared_ptr<Scene> &scene);

private:
  GPUBufferState bufferState;

  std::vector<uint32_t> m_primitivesToUpdate;
};

}