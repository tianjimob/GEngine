#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/delegates/delegate.h"
#include "core/math/vector3.h"
#include "core/reflection/reflection.h"
#include "function/framework/component/component.h"
#include "function/framework/input/input_types.h"
#include "function/framework/input/key.h"

namespace GEngine {

struct InputBinding {
  /** Whether the binding should consume the input or allow it to pass to
   * another component */
  bool consumeInput;
  
  InputBinding() : consumeInput(true) {}
};

struct ModifierIndicator {
  bool shift;
  bool ctrl;
  bool alt;
  bool cmd;

  ModifierIndicator() : shift(false), ctrl(false), alt(false), cmd(false) {}
  ModifierIndicator(bool shift, bool ctrl, bool alt, bool cmd)
      : shift(shift), ctrl(ctrl), alt(alt), cmd(cmd) {}
};

DECLARE_DELEGATE(InputKeyDelegate, void);
DECLARE_DELEGATE(InputKeyDelegateWithKey, void, Key);

struct InputKeyBinding : public InputBinding {
  InputEvent inputEvent;
  Key key;
  ModifierIndicator modifier;

  union {
    std::shared_ptr<InputKeyDelegate> inputKeyDelegate;
    std::shared_ptr<InputKeyDelegateWithKey> inputKeyDelegateWithKey;
  };
  enum class BoundDelegate { Unbound, Delegate, DelegateWithKey };
  BoundDelegate boundDelegate;

  InputKeyBinding(InputEvent event, const Key &key)
      : inputEvent(event), key(key) {}

  InputKeyBinding(InputEvent event, const Key &key, ModifierIndicator indicator)
      : inputEvent(event), key(key), modifier(indicator) {}
};

DECLARE_DELEGATE(InputAxisDelegate, void, float);

struct InpuAxisKeyBinding : public InputBinding {
  Key axisKey;
  std::shared_ptr<InputAxisDelegate> inputAxisDelegate;

  InpuAxisKeyBinding(const Key &key) : axisKey(key) {}
};

struct InpuAxisBinding : public InputBinding {
  std::string axisName;
  std::shared_ptr<InputAxisDelegate> inputAxisDelegate;

  InpuAxisBinding(const std::string &axisName) : axisName(axisName) {}
};

DECLARE_DELEGATE(InputVectorAxisDelegate, void, Vector3);

struct InputVectorAxisBinding : public InputBinding {
  Key axisKey;
  std::shared_ptr<InputVectorAxisDelegate> inputVectorAxisDelegate;

  InputVectorAxisBinding(const Key &key) : axisKey(key) {}
};

struct InputActionBinding : public InputBinding {
  std::string actionName;
  InputEvent event;
  union {
    std::shared_ptr<InputKeyDelegate> inputKeyDelegate;
    std::shared_ptr<InputKeyDelegateWithKey> inputKeyDelegateWithKey;
  };
  enum class BoundDelegate { Unbound, Delegate, DelegateWithKey };
  BoundDelegate boundDelegate;

  InputActionBinding(const std::string &actionName, InputEvent inputEvent)
      : actionName(actionName), event(inputEvent) {}
};

CLASS(InputComponent) : public ActorComponent {
  REFLECTION_BODY(InputComponent)

 public:
  template <typename Class>
  InputKeyBinding &bindKey(const Key &key, const InputEvent &inputEvent,
                           const ModifierIndicator &modifierIndicator,
                           Class *object,
                           InputKeyDelegate::MethodPtr<Class> func) {
    auto &binding =
        m_keyBindings.emplace_back(inputEvent, key, modifierIndicator);
    binding.inputKeyDelegate = std::make_shared<InputKeyDelegate>();
    binding.inputKeyDelegate->bindMemberFunction(object, func);
    binding.boundDelegate = InputKeyBinding::BoundDelegate::Delegate;
    return binding;
  }

  template <typename Class>
  InputKeyBinding &bindKey(const Key &key, const InputEvent &inputEvent,
                           const ModifierIndicator &modifierIndicator,
                           Class *object,
                           InputKeyDelegateWithKey::MethodPtr<Class> func) {
    auto &binding =
        m_keyBindings.emplace_back(inputEvent, key, modifierIndicator);
    binding.inputKeyDelegateWithKey =
        std::make_shared<InputKeyDelegateWithKey>();
    binding.inputKeyDelegateWithKey->bindMemberFunction(object, func);
    binding.boundDelegate = InputKeyBinding::BoundDelegate::DelegateWithKey;
    return binding;
  }

  template <typename Class>
  InpuAxisKeyBinding &bindAxisKey(const Key &key, Class *object,
                                  InputAxisDelegate::MethodPtr<Class> func) {
    auto &binding = m_axisKeyBindings.emplace_back(key);
    binding.inputAxisDelegate = std::make_shared<InputAxisDelegate>();
    binding.inputAxisDelegate->bindMemberFunction(object, func);
    return binding;
  }

  template <typename Class>
  InpuAxisBinding &bindAxis(const std::string &axisName, Class *object,
                            InputAxisDelegate::MethodPtr<Class> func) {
    auto &binding = m_axisBindings.emplace_back(axisName);
    binding.inputAxisDelegate = std::make_shared<InputAxisDelegate>();
    binding.inputAxisDelegate->bindMemberFunction(object, func);
    return binding;
  }

  template <typename Class>
  InputVectorAxisBinding &bindVectorAxis(
      const Key &axisKey, Class *object,
      InputAxisDelegate::MethodPtr<Class> func) {
    auto &binding = m_vectorAxisKeyBindings.emplace_back(axisKey);
    binding.inputVectorAxisDelegate =
        std::make_shared<InputVectorAxisDelegate>();
    binding.inputVectorAxisDelegate->bindMemberFunction(object, func);
    return binding;
  }

  template <typename Class>
  InputActionBinding &bindAction(const std::string &actionName,
                                 const InputEvent &inputEvent, Class *object,
                                 InputKeyDelegate::MethodPtr<Class> func) {
    auto &binding = m_actionBindings.emplace_back(actionName, inputEvent);
    binding.inputKeyDelegate = std::make_shared<InputKeyDelegate>();
    binding.inputKeyDelegate->bindMemberFunction(object, func);
    binding.boundDelegate = InputActionBinding::BoundDelegate::Delegate;
    return binding;
  }

  template <typename Class>
  InputActionBinding &bindAction(
      const std::string &actionName, const InputEvent &inputEvent,
      Class *object, InputKeyDelegateWithKey::MethodPtr<Class> func) {
    auto &binding = m_actionBindings.emplace_back(actionName, inputEvent);
    binding.inputKeyDelegateWithKey =
        std::make_shared<InputKeyDelegateWithKey>();
    binding.inputKeyDelegateWithKey->bindMemberFunction(object, func);
    binding.boundDelegate = InputActionBinding::BoundDelegate::DelegateWithKey;
    return binding;
  }

  auto &getKeyBindings() { return m_keyBindings; }
  auto &getAxisKeyBindings() { return m_axisKeyBindings; }
  auto &getAxisBindings() { return m_axisBindings; }
  auto &getVectorAxisBindings() { return m_vectorAxisKeyBindings; }
  auto &getActionBindings() { return m_actionBindings; }

 private:
  std::vector<InputKeyBinding> m_keyBindings;
  std::vector<InpuAxisKeyBinding> m_axisKeyBindings;
  std::vector<InpuAxisBinding> m_axisBindings;
  std::vector<InputVectorAxisBinding> m_vectorAxisKeyBindings;
  std::vector<InputActionBinding> m_actionBindings;
};

}  // namespace GEngine