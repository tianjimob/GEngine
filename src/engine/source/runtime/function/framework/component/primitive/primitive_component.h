#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include "function/framework/component/transform/transform_component.h"
#include <atomic>
#include <memory>

namespace GEngine {

CLASS(PrimitiveComponent) : public TransformComponent {
  REFLECTION_BODY(PrimitiveComponent)
public:
  PrimitiveComponent();

  virtual std::shared_ptr<PrimitiveSceneProxy> createSceneProxy() {
    return std::make_shared<PrimitiveSceneProxy>();
  }

  void setPrimitiveSceneProxy(std::shared_ptr<PrimitiveSceneProxy> &
                              primitiveSceneProxy) {
    sceneProxy = primitiveSceneProxy;
  }

  std::weak_ptr<PrimitiveSceneProxy> getPrimitiveSceneProxy() {
    return sceneProxy;
  }

  uint32_t getId() const { return m_primitiveComponentId; }

protected:
  std::shared_ptr<PrimitiveSceneProxy> sceneProxy;

private:
  static std::atomic_uint32_t s_nextId;

  uint32_t m_primitiveComponentId;
};

}