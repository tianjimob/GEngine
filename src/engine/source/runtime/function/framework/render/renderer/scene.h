#pragma once

#include "core/math/matrix4.h"
#include "core/math/box_sphere_bounds.h"
#include "function/framework/component/primitive/primitive_component.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include "function/framework/render/renderer/gpu_scene.h"
#include <memory>
#include <unordered_set>
#include <vector>

namespace GEngine {

class Scene : public std::enable_shared_from_this<Scene> {
public:
  void addPrimitive(PrimitiveComponent &primitive);
  void updateAllPrimitiveInfos();

public:
  std::unordered_set<PrimitiveSceneInfo*> addedPrimitiveInfos;

  std::vector<PrimitiveSceneInfo *> primitives;
  std::vector<Matrix4x4> primitiveTransforms;
  std::vector<BoxSphereBounds> bounds;
  std::vector<uint32_t> primitiveComponentIds;
  GPUScene gpuScene;
};

}