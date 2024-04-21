#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/actor/actor.h"
// #include "function/framework/actor/level_setting/level_setting.h"

#include <memory>
#include <string>
#include <vector>

namespace GEngine {

class World;
  
CLASS(Level) : public GObject {

  REFLECTION_BODY(Level)
  
public:
  void tick(float deltaTime);

  const std::string& getLevelUrl() const { return m_path; }
  void setLevelPath(const std::string &path) { m_path = path; }

  virtual void postLoad(std::weak_ptr<GObject> parentObject) override {
    if (auto object = parentObject.lock(); object->isA<World>()) {
      m_world = std::static_pointer_cast<World>(object);
    }
  }

  auto &getActos() { return m_actors; }
  auto getWorld() { return m_world; }

private:
  std::string m_path;

  // pointer to world this level belong to
  std::weak_ptr<World> m_world;

  META_FIELD()
  std::vector<std::shared_ptr<Actor>> m_actors;

  // META_FIELD()
  // std::shared_ptr<LevelSetting> m_levelSetting;
};

}