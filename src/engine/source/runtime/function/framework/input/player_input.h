#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "key.h"
#include "key_state.h"

#include <map>
#include <vector>

namespace GEngine {

class InputComponent;

CLASS(PlayerInput) : public GObject {
  REFLECTION_BODY(PlayerInput)
  
public:
  virtual ~PlayerInput() = default;
  
  void tickInputStack(const std::vector<InputComponent *> &inputStack,
                      float deltaTime);

  virtual bool inputKey(Key key, InputEvent event, Vector3 delta = {0, 0, 0});

private:
  std::map<Key, KeyState> m_keyStates;
};

}