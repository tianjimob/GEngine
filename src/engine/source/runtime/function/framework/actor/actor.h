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

class InputComponent;
class Level;
class PlayerController;

CLASS(Actor) : public GObject {

  REFLECTION_BODY(Actor)

public:
  virtual ~Actor() = default;
  virtual void tick(float deltaTime) {}
  virtual void postLoad(std::weak_ptr<GObject> parentObject) override {
    if (auto object = parentObject.lock(); object->isA<Level>()) {
      m_level = std::static_pointer_cast<Level>(object);
    }
  }

  void init();

  META_FUNCTION()
  const Transform &getTransform() const {return {};}

  META_FUNCTION()
  const Transform &actorToWorld() const {return {};}

  std::shared_ptr<TransformComponent> getRootComponent() const { return {}; }

  std::shared_ptr<InputComponent> getInputComponent() {
    return m_inputComponent;
  }

  auto &getComponents() { return m_ownedComponents; }
  auto getLevel() { return m_level; }

protected:
  std::vector<std::weak_ptr<Actor>> m_children;

  META_FIELD()
  std::shared_ptr<TransformComponent> m_rootComponent;

  std::shared_ptr<InputComponent> m_inputComponent;

private:
  // pointer to level this actor belong to
  std::weak_ptr<Level> m_level;

  META_FIELD()
  std::set<std::shared_ptr<ActorComponent>> m_ownedComponents;
};

} // namespace GEngine