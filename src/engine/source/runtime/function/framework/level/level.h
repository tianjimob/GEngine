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
public:
  void tick(float deltaTime);

private:
  std::string m_path;
  
  std::vector<std::shared_ptr<Actor>> m_actors;

  // META_FIELD()
  // std::shared_ptr<LevelSetting> m_levelSetting;
};

}