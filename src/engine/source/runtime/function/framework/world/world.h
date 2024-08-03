#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/level/level.h"

#include <memory>
#include <string>
#include <vector>

namespace GEngine {

class GameInstance;
class GameViewportClient;
class PlayerController;
class Player;

STRUCT(WorldInitializer) : public GObject {

  REFLECTION_BODY(WorldInitializer)

  META_FIELD()
  std::string name;

  META_FIELD()
  std::vector<std::string, std::allocator<std::string>> levelUrls;

  META_FIELD()
  std::string defautLevelUrl;
};

CLASS(World) : public GObject {

  REFLECTION_BODY(World)

public:
  void load(const WorldInitializer& worldInitializer, std::weak_ptr<GameInstance> gameInstance);
  void tick(float deltaTime);

  void addLevel(std::shared_ptr<Level> & level) {
    m_levels.emplace_back(level);
  }

  bool setCurrentLevel(const std::string &levelUrl);
  void setGameViewport(std::weak_ptr<GameViewportClient> gameViewport) {
    m_gameViewport = gameViewport;
  }

  std::shared_ptr<PlayerController> spawnPlayActor(Player * player);

  auto getGameInstance() { return m_owingGameInstance; }

private:
  
  std::vector<std::shared_ptr<Level>> m_levels; // all loaded levels

  std::weak_ptr<Level> m_currentLevel;

  // pointer to GameInstance created this world
  std::weak_ptr<GameInstance> m_owingGameInstance;

  // pointer to GameViewportClient created by GameInstance who's belong to GameEngine
  std::weak_ptr<GameViewportClient> m_gameViewport;

  // std::shared_ptr<GameMode> m_gameMode;

  // std::shared_ptr<GameState> m_gameState;
};

}