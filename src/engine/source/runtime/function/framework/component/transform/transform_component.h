#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/component/component.h"
#include "core/math/transform.h"

#include <memory>
#include <vector>

namespace GEngine {

CLASS(TransformComponent) : public ActorComponent {
  REFLECTION_BODY(TransformComponent)
  
public:
private:
  META_FIELD()
  std::weak_ptr<TransformComponent> m_attchParent;

  META_FIELD()
  std::vector<std::shared_ptr<TransformComponent>> m_attachChildren;

  Transform m_componentToWorld;

//   BoxSphereBounds m_bounds;
};

}