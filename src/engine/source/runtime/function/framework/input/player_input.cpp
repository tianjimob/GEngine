#include "player_input.h"

#include <utility>
#include <vector>

#include "function/framework/component/input/input_component.h"
#include "function/framework/input/key.h"
#include "input_settings.h"
#include "input_types.h"


namespace GEngine {

void PlayerInput::tickInputStack(
    const std::vector<InputComponent *> &inputStack, float deltaTime) {
  
  static std::vector<std::pair<Key *, KeyState *>> keysWithEvents;

  for (const auto &[key, keyState] : m_keyStates) {
    for (int i = 0; i < static_cast<int>(InputEvent::Num); ++i) {
      if (keyState.count[i] > 0) {
        keysWithEvents.emplace_back(&key, &keyState);
        break;
      }
    }
  }

  auto itEnd = inputStack.rend();
  for (auto it = inputStack.rbegin(); it != itEnd; ++it) {
    if (*it) {
      InputComponent *component = *it;

      for (auto &binding : component->getActionBindings()) {
        std::vector<InputActionKeyMapping> actionKeyMappings;
        InputSettings::instance().getActionsMappingByName(binding.actionName,
                                                          actionKeyMappings);

        for (const auto [key, keyState] : keysWithEvents) {
          if (keyState->consumed) continue;

          auto it =
              std::find_if(actionKeyMappings.begin(), actionKeyMappings.end(),
                           [&binding](const InputActionKeyMapping &mapping) {
                             return binding.actionName == mapping.actionName;
                           });
          if (it == actionKeyMappings.end()) continue;

          if (*key != it->key) continue;

          if (keyState->count[static_cast<int>(binding.event)] == 0) continue;

          auto isKeyPressed = [this](const Key &key) {
            if (m_keyStates.count(Keys::Shift) == 0)
              return false;
            else if (m_keyStates[Keys::Shift]
                         .count[static_cast<int>(InputEvent::Pressed)] == 0)
              return false;
            return true;
          };

          if (it->shift && !isKeyPressed(Keys::Shift)) continue;
          if (it->ctrl && !isKeyPressed(Keys::Ctrl)) continue;
          if (it->alt && !isKeyPressed(Keys::Alt)) continue;
          if (it->cmd && !isKeyPressed(Keys::Cmd)) continue;

          if (binding.consumeInput) keyState->consumed = true;

          if (binding.boundDelegate ==
              InputActionBinding::BoundDelegate::Delegate) {
            binding.inputKeyDelegate->safeExecute();
          } else if (binding.boundDelegate ==
                     InputActionBinding::BoundDelegate::DelegateWithKey) {
            binding.inputKeyDelegateWithKey->safeExecute(*key);
          }
        }
      }

      for (auto &binding : component->getKeyBindings()) {
        for (const auto [key, keyState] : keysWithEvents) {
          if (keyState->consumed) continue;

          if (*key != binding.key) continue;

          if (keyState->count[static_cast<int>(binding.inputEvent)] == 0)
            continue;

          auto isKeyPressed = [this](const Key &key) {
            if (m_keyStates.count(Keys::Shift) == 0)
              return false;
            else if (m_keyStates[Keys::Shift]
                         .count[static_cast<int>(InputEvent::Pressed)] == 0)
              return false;
            return true;
          };

          auto &modifier = binding.modifier;
          if (modifier.shift && !isKeyPressed(Keys::Shift)) continue;
          if (modifier.ctrl && !isKeyPressed(Keys::Ctrl)) continue;
          if (modifier.alt && !isKeyPressed(Keys::Alt)) continue;
          if (modifier.cmd && !isKeyPressed(Keys::Cmd)) continue;

          if (binding.consumeInput) keyState->consumed = true;

          if (binding.boundDelegate ==
              InputKeyBinding::BoundDelegate::Delegate) {
            binding.inputKeyDelegate->safeExecute();
          } else if (binding.boundDelegate ==
                     InputKeyBinding::BoundDelegate::DelegateWithKey) {
            binding.inputKeyDelegateWithKey->safeExecute(*key);
          }
        }
      }

      for (auto &binding : component->getAxisBindings()) {
        std::vector<InputAxisKeyMapping> axisKeyMappings;
        InputSettings::instance().getAxisMappingByName(binding.axisName,
                                                       axisKeyMappings);

        for (const auto [key, keyState] : keysWithEvents) {
          if (keyState->consumed) continue;

          auto it =
              std::find_if(axisKeyMappings.begin(), axisKeyMappings.end(),
                           [&binding](const InputAxisKeyMapping &mapping) {
                             return binding.axisName == mapping.axisName;
                           });
          if (it == axisKeyMappings.end()) continue;

          if (*key != it->key) continue;

          if (binding.consumeInput) keyState->consumed = true;

          if (binding.inputAxisDelegate) {
            binding.inputAxisDelegate->safeExecute(it->scale *
                                                   keyState->rawValue.x);
          }
        }
      }

      for (auto &binding : component->getAxisKeyBindings()) {
        for (const auto [key, keyState] : keysWithEvents) {
          if (keyState->consumed) continue;

          if (*key != binding.axisKey) continue;

          if (binding.consumeInput) keyState->consumed = true;

          if (binding.inputAxisDelegate) {
            binding.inputAxisDelegate->safeExecute(keyState->rawValue.x);
          }
        }
      }

      for (auto &binding : component->getVectorAxisBindings()) {
        for (const auto [key, keyState] : keysWithEvents) {
          if (keyState->consumed) continue;

          if (*key != binding.axisKey) continue;

          if (binding.consumeInput) keyState->consumed = true;

          if (binding.inputVectorAxisDelegate) {
            binding.inputVectorAxisDelegate->safeExecute(keyState->rawValue);
          }
        }
      }
    }
  }

  m_keyStates.clear();
  keysWithEvents.clear();
}

bool PlayerInput::inputKey(Key key, InputEvent event, Vector3 delta) {
  if (key.isAnalog()) {
    auto &keyState = m_keyStates[key];
    keyState.rawValue += delta;
    return true;
  } else {
    auto &keyState = m_keyStates[key];
    switch (event) {
      case InputEvent::Pressed:
      case InputEvent::Repeat:
        keyState.rawValue.x =
            delta.x;  // rawValue.x is number of presses for pressed or repeat
        ++keyState.count[static_cast<int>(event)];
        break;
      case InputEvent::Released:
        keyState.rawValue.x = 0;
        ++keyState.count[static_cast<int>(event)];
        break;
      case InputEvent::DoubleClick:
        keyState.rawValue.x = delta.x;
        ++keyState.count[static_cast<int>(InputEvent::Pressed)];
        ++keyState.count[static_cast<int>(InputEvent::DoubleClick)];
        break;
      default:
        break;
    }
    return true;
  }
  return false;
}

}  // namespace GEngine