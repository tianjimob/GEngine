#pragma once

#include "function/framework/component/component.h"
#include "function/framework/actor/actor.h"
#include <memory>

namespace GEngine {

void ActorComponent::postLoad(std::weak_ptr<GObject> parentObject) {
  if (auto object = parentObject.lock(); object->isA<Actor>()) {
    m_owner = std::static_pointer_cast<Actor>(object);
    if (auto actor = m_owner.lock()) {
      actor->addComponent(std::static_pointer_cast<ActorComponent>(shared_from_this()));
    }
  }
}

void ActorComponent::onSerializeRead() {
  if (getOuter() && getOuter()->isA<Actor>()) {
    auto *actor = (Actor *)getOuter();
    actor->addComponent(
        std::static_pointer_cast<ActorComponent>(shared_from_this()));
  }
}

}  // namespace GEngine