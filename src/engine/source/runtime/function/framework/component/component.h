#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include <memory>

namespace GEngine {

class Actor;

CLASS(ActorComponent) : public GObject {

  REFLECTION_BODY(ActorComponent)

public:
  virtual ~ActorComponent() = default;

  virtual void tick(float deltaTime) {}
  virtual void postLoad(std::weak_ptr<GObject> parentObject) override {
    if (auto object = parentObject.lock(); object->isA<Actor>()) {
      m_owner = std::static_pointer_cast<Actor>(object);
    }
  }

private:
  std::weak_ptr<Actor> m_owner;
//   std::weak_ptr<World> m_world;
};

} // namespace GEngine