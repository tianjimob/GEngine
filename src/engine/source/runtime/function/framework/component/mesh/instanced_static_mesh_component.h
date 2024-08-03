#pragma once

#include "core/math/transform.h"
#include "core/reflection/reflection.h"
#include "function/framework/component/mesh/static_mesh_component.h"
#include <memory>
#include <vector>
namespace GEngine {

CLASS(InstancedStaticMeshComponent) : public StaticMeshComponent {
public:
  virtual std::shared_ptr<PrimitiveSceneProxy> createSceneProxy() override;

  std::vector<Transform>& getPerInstanceTransforms() { return m_perInstanceTransforms; }
private:
  std::vector<Transform> m_perInstanceTransforms;
};

}