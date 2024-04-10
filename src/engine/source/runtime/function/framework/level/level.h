#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/actor/actor.h"
// #include "function/framework/actor/level_setting/level_setting.h"

#include <memory>
#include <string>
#include <vector>

namespace GEngine {
  
CLASS(Level) : public GObject {

  REFLECTION_BODY(Level)
  
public:
  void tick(float deltaTime);

  const std::string& getLevelUrl() const { return m_path; }
  void setLevelPath(const std::string &path) {
    m_path = path;
  }

private:
  std::string m_path;

  META_FIELD()
  std::vector<std::shared_ptr<Actor>> m_actors;

  // META_FIELD()
  // std::shared_ptr<LevelSetting> m_levelSetting;
};

}