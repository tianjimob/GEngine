#include "actor.h"

#include "function/framework/actor/controller/player_controller/player_controller.h"

namespace GEngine {

void Actor::init() {
  if (m_inputComponent) {
    auto *controller = PlayerController::GetPlayControllerFromActor(this);
    if (controller) {
      controller->pushInputComponent(m_inputComponent.get());
    }
  }
}

}