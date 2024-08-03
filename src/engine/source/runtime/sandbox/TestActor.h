#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"
#include "function/framework/component/transform/transform_component.h"
#include <memory>
namespace GEngine {

CLASS(TestActor) : public Actor {
  REFLECTION_BODY(TestActor)

public:
  TestActor();
private:
  META_FIELD()
  std::shared_ptr<TransformComponent> m_transformComponent;
};

}