#pragma once

#include "function/framework/component/primitive/primitive_component.h"
#include "function/framework/render/renderer/scene.h"
#include "primitive_scene_proxy.h"
#include <cstdint>
#include <memory>

namespace GEngine {

class PrimitiveSceneInfo {
public:
  PrimitiveSceneInfo(PrimitiveComponent& component, std::shared_ptr<Scene> scene);
private:
  std::weak_ptr<PrimitiveSceneProxy> m_proxy;
  std::weak_ptr<Scene> m_scene;
  uint32_t m_primitiveComponentId;
  int32_t m_instanceSceneDataOffset;
  int32_t m_numInstanceSceneDataEntries;
};

}