#pragma once

#include "core/math/matrix4.h"
#include "core/math/box_sphere_bounds.h"
#include "function/framework/component/primitive/primitive_component.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include "function/framework/render/core/rendering_thread.h"
#include "function/framework/render/renderer/gpu_scene.h"
#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

namespace GEngine {

class World;
class Scene : public std::enable_shared_from_this<Scene> {
public:
  Scene(std::shared_ptr<World> world);

  void addPrimitive(PrimitiveComponent &primitive);
  void removePrimitive(PrimitiveComponent &primitive);
  void updateAllPrimitiveInfos();

  void incrementFrameNumber() { ++m_sceneFrameNumber; }
  uint32_t getFrameNumber() {
    return m_sceneFrameNumber;
  }

public:
  std::weak_ptr<World> world;

  // changed by removePrimitive()
  std::unordered_set<PrimitiveSceneInfo*> removededPrimitiveInfos;

  // changed by addPrimitive()
  std::unordered_set<PrimitiveSceneInfo *> addedPrimitiveInfos;

public:
  // updated by updateAllPrimitiveInfos()
  
  std::vector<PrimitiveSceneInfo *> primitives;
  std::vector<Matrix4x4> primitiveTransforms;
  std::vector<BoxSphereBounds> bounds;
  std::vector<uint32_t> primitiveComponentIds;
  GPUScene gpuScene;

private:
  uint32_t m_sceneFrameNumber;
};

}