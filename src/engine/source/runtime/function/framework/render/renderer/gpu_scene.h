#pragma once

#include "function/framework/render/renderer/scene.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>

namespace GEngine {

class Scene;

struct GPUBufferState {
  std::shared_ptr<RHIBuffer> primitiveBuffer;
};

class GPUScene {
public:
  void updateBufferState(std::shared_ptr<Scene> &scene);

private:
  GPUBufferState bufferState;
};

}