#pragma once

#include "core/math/transform.h"
#include "core/reflection/reflection.h"
#include "function/framework/component/child_actor/child_actor_component.h"
#include "function/framework/component/component.h"
#include "function/framework/component/transform/transform_component.h"
#include "function/framework/object/object.h"


#include <memory>
#include <set>
#include <vector>

namespace GEngine {

CLASS(Actor) : public GObject {

  REFLECTION_BODY(Actor)

public:
  virtual ~Actor() = default;
  virtual void tick(float deltaTime) {}

  META_FUNCTION()
  const Transform &getTransform() const {return {};}

  META_FUNCTION()
  const Transform &actorToWorld() const {return {};}

  std::shared_ptr<TransformComponent> getRootComponent() const {return {};}

protected:
  std::vector<std::weak_ptr<Actor>> m_children;

  std::shared_ptr<TransformComponent> m_rootComponent;

private:
  std::weak_ptr<ChildActorComponent> m_parentComponent;

  std::set<std::shared_ptr<ActorComponent>> m_ownedComponents;
};

} // namespace GEngine