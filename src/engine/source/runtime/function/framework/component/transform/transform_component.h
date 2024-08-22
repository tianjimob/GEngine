#pragma once

#include "core/math/matrix4.h"
#include "core/math/quaternion.h"
#include "core/math/rotator.h"
#include "core/reflection/reflection.h"
#include "function/framework/component/component.h"
#include "core/math/transform.h"

#include <memory>
#include <optional>
#include <vector>

namespace GEngine {

CLASS(TransformComponent) : public ActorComponent {
  REFLECTION_BODY(TransformComponent)

public:
  const Rotator& getComponentRatator();
protected:
  META_FIELD()
  Transform m_transformToWorld;

  // m_cachedRotator <--- m_cachedQuat <-- m_transformToWorld.ratation
  Quaternion m_cachedQuat;
  Rotator m_cachedRotator;
};

}