#include "pawn.h"

#include <function/framework/component/input/input_component.h>

namespace GEngine {

void Pawn::resetInputComponent() {
  m_inputComponent.reset();
  m_inputComponent = std::make_shared<InputComponent>();
  setupPlayerInputComponent(m_inputComponent);
}
}