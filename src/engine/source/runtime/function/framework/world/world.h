#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/reflection/reflection.h"
#include "function/framework/actor/game_mode/game_mode.h"
#include "function/framework/level/level.h"
#include "function/framework/object/object.h"


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

  META_FIELD()
  GameMode gameMode;
};

CLASS(World) : public GObject {
  REFLECTION_BODY(World)

 public:
  World() {
    m_persistentLevel = std::make_shared<Level>();
    m_persistentLevel->postLoad(shared_from_this());
  }

  void load(const WorldInitializer& worldInitializer,
            std::weak_ptr<GameInstance> gameInstance);
  void tick(float deltaTime);

  void addLevel(std::shared_ptr<Level> & level) {
    m_levels.emplace_back(level);
  }

  bool setCurrentLevel(const std::string& levelUrl);
  void setGameViewport(std::weak_ptr<GameViewportClient> gameViewport) {
    m_gameViewport = gameViewport;
  }

  std::shared_ptr<PlayerController> spawnPlayActor(Player * player);

  auto getGameInstance() { return m_owingGameInstance; }

 private:
  std::shared_ptr<Level> m_persistentLevel;

  std::vector<std::shared_ptr<Level>> m_levels;  // all loaded levels

  std::weak_ptr<Level> m_currentLevel;

  // pointer to GameInstance created this world
  std::weak_ptr<GameInstance> m_owingGameInstance;

  // pointer to GameViewportClient created by GameInstance who's belong to
  // GameEngine
  std::weak_ptr<GameViewportClient> m_gameViewport;

  GameMode m_gameMode;

  // std::shared_ptr<GameState> m_gameState;
};

}  // namespace GEngine