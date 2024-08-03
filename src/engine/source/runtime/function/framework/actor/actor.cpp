#include "actor.h"

#include "function/framework/actor/controller/player_controller/player_controller.h"
#include "function/framework/level/level.h"
#include "function/framework/object/object.h"
#include <memory>

namespace GEngine {

void Actor::init() {
  if (m_inputComponent) {
    auto *controller = PlayerController::GetPlayControllerFromActor(this);
    if (controller) {
      controller->pushInputComponent(m_inputComponent.get());
    }
  }
}

void Actor::postLoad(std::weak_ptr<GObject> parentObject) {
  if (auto object = parentObject.lock(); object->isA<Level>()) {
    m_level = std::static_pointer_cast<Level>(object);
  }
}

void Actor::addComponent(std::weak_ptr<ActorComponent> componet) {
  if (componet.lock() &&
      m_ownedComponents.find(componet) == m_ownedComponents.end()) {
    m_ownedComponents.emplace(componet);
  }
}
}