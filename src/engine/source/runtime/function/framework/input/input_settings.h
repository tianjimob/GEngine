#pragma once

#include <string>
#include <vector>

#include "core/reflection/reflection.h"
#include "function/framework/input/key.h"
#include "function/framework/object/object.h"

namespace GEngine {

STRUCT(InputAxisKeyMappingInitializer) : public GObject {
  REFLECTION_BODY(InputAxisKeyMappingInitializer)

  META_FIELD()
  std::string axisName;

  META_FIELD()
  std::string key;

  META_FIELD()
  float scale;
};

STRUCT(InputAxisKeyMapping) : public GObject {
  REFLECTION_BODY(InputAxisKeyMapping)

  InputAxisKeyMapping() = default;

  InputAxisKeyMapping(InputAxisKeyMappingInitializer & initializer)
      : axisName(initializer.axisName),
        key(initializer.key),
        scale(initializer.scale) {}

  META_FIELD()
  std::string axisName;

  META_FIELD()
  Key key;

  META_FIELD()
  float scale;

  bool operator==(const InputAxisKeyMapping &right) const {
    return axisName == right.axisName && key == right.key &&
           scale == right.scale;
  }

  virtual void postLoad(std::weak_ptr<GObject> parentObject) override {
    key.postLoad({});
  }
};

STRUCT(InputActionKeyMappingInitializer) : public GObject {
  REFLECTION_BODY(InputActionKeyMappingInitializer)

  META_FIELD()
  std::string actionName;

  META_FIELD()
  std::string key;

  META_FIELD()
  bool shift;

  META_FIELD()
  bool ctrl;

  META_FIELD()
  bool alt;

  META_FIELD()
  bool cmd;
};

STRUCT(InputActionKeyMapping) : public GObject {
  REFLECTION_BODY(InputActionKeyMapping)

  InputActionKeyMapping() = default;

  InputActionKeyMapping(InputActionKeyMappingInitializer & initializer)
      : actionName(initializer.actionName),
        key(initializer.key),
        shift(initializer.shift),
        ctrl(initializer.ctrl),
        alt(initializer.alt),
        cmd(initializer.cmd) {}

  META_FIELD()
  std::string actionName;

  META_FIELD()
  Key key;

  META_FIELD()
  bool shift;

  META_FIELD()
  bool ctrl;

  META_FIELD()
  bool alt;

  META_FIELD()
  bool cmd;

  bool operator==(const InputActionKeyMapping &right) const {
    return actionName == right.actionName && key == right.key &&
           shift == right.shift && ctrl == right.ctrl && alt == right.alt &&
           cmd == right.cmd;
  }

  virtual void postLoad(std::weak_ptr<GObject> parentObject) override {
    key.postLoad({});
  }
};

class InputSettings {
 public:
  static InputSettings &instance() {
    static InputSettings instance;
    return instance;
  }

  void addAxisKeyMapping(const InputAxisKeyMapping &axisKeyMapping);

  void addActionKeyMapping(const InputActionKeyMapping &actionKeyMapping);

  void getAxisMappingByName(const std::string &axisName,
                            std::vector<InputAxisKeyMapping> &outMappings);

  void getActionsMappingByName(const std::string &actionName,
                               std::vector<InputActionKeyMapping> &outMappings);

 private:
  std::vector<InputAxisKeyMapping> m_axisMappings;
  std::vector<InputActionKeyMapping> m_actionMappings;
};

}  // namespace GEngine