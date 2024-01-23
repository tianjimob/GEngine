#pragma once

#include <memory>
#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/component/transform/transform_component.h"

namespace GEngine {

class Actor;

CLASS(ChildActorComponent) : public TransformComponent {

  REFLECTION_BODY(ChildActorComponent)
  
public:
private:
  std::shared_ptr<Actor> m_childActor;
};

}