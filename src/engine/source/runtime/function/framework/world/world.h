#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/level/level.h"

#include <memory>
#include <vector>

namespace GEngine {

class GameInstance;

CLASS(World) : public GObject {

  REFLECTION_BODY(World)

public:
  void tick(float deltaTime);

private:
  META_FIELD()
  std::vector<std::shared_ptr<Level>> m_levels; // all loaded levels

  std::weak_ptr<Level> m_currentLevel;

  META_FIELD()
  std::shared_ptr<Level> m_persistentLevel;

  std::weak_ptr<GameInstance> m_owingGameInstance;

  // std::shared_ptr<GameMode> m_gameMode;

  // std::shared_ptr<GameState> m_gameState;
};

}