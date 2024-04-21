#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"

#include <memory>

namespace GEngine {

class Controller;
class InputComponent;

CLASS(Pawn) : public Actor {

  REFLECTION_BODY(Pawn)

public:
  virtual void setupPlayerInputComponent(std::shared_ptr<InputComponent> inputComponent) {}

  void resetInputComponent() {
    m_inputComponent.reset();
    m_inputComponent = std::make_shared<InputComponent>();
    setupPlayerInputComponent(m_inputComponent);
  }

private:
  //   std::weak_ptr<PlayerState> m_playerState;
  std::weak_ptr<Controller> m_controller;
};

} // namespace GEngine