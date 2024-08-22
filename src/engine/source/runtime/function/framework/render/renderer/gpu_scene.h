#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace GEngine {

class Scene;

enum class PrimitiveDirtyState {
  None = 0,
  ChangedId = 1,
  ChangedTransform = 1 << 1,
};
struct GPUBufferState {
  std::shared_ptr<RHIBuffer> primitiveBuffer;
};

class GPUScene {
public:
  void update(std::shared_ptr<Scene> &scene);
  void updateBufferState(std::shared_ptr<Scene> &scene);
  void upload(std::shared_ptr<Scene> &scene);

private:
  GPUBufferState bufferState;

  std::vector<uint32_t> m_primitivesToUpdate;
  std::vector<PrimitiveDirtyState> m_primitivesDirtyState;
};

}