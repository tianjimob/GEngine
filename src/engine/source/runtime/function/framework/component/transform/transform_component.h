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
protected:
  META_FIELD()
  Transform m_transformToWorld;

//   BoxSphereBounds m_bounds;
};

}