#include "world.h"

#include "core/log/logger.h"

#include "_generated/serializer/all_serializer.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include "function/framework/actor/pawn/pawn.h"

#include <memory>
#include <fstream>

namespace GEngine {

DECLARE_LOG_CATEGORY(LogWorld)

void World::load(const WorldInitializer &worldInitializer, std::weak_ptr<GameInstance> gameInstance) {
  for (auto &levelUrl : worldInitializer.levelUrls) {
    std::ifstream levelJson{levelUrl};
    if (!levelJson) {
      LOG_ERROR(LogWorld, "failed to load file {} at {}:{}",
                levelUrl, __FILE__, __LINE__);
      continue;
    }
    Json data = Json::parse(levelJson);
    std::shared_ptr<Level> level = std::make_shared<Level>();
    Serializer::read(data, *level);
    level->postLoad(weak_from_this());
    for (auto &actor : level->getActos()) {
      actor->postLoad(level);
      for (auto &component : actor->getComponents()) {
        component->postLoad(actor);
      }
    }
    addLevel(level);
  }
  setCurrentLevel(worldInitializer.defautLevelUrl);
  m_owingGameInstance = gameInstance;

  
  PlayerController* controller;
  if (auto level = m_currentLevel.lock()) {
    for (auto &actor : level->getActos()) {
      if (actor->isA<Pawn>()) {
        if (controller == nullptr)
          controller =
              PlayerController::GetPlayControllerFromActor(actor.get());
        if (controller) {
          auto pawn = std::static_pointer_cast<Pawn>(actor);
          controller->setPawn(pawn);
          pawn->resetInputComponent();
        }
      }
    }
  }
}

void World::tick(float deltaTime) {
  std::shared_ptr<Level> level = m_currentLevel.lock();
  if (level) {
    level->tick(deltaTime);
  }
}

bool World::setCurrentLevel(const std::string& levelUrl){
  for (auto &level : m_levels) {
    if (level->getLevelUrl() == levelUrl) {
      m_currentLevel = level;
      return true;
    }
  }
  return false;
}

std::shared_ptr<PlayerController> World::spawnPlayActor(Player *player) {
  std::shared_ptr<PlayerController> retController =
      std::make_shared<PlayerController>();
  retController->setWorld(std::static_pointer_cast<World>(shared_from_this()));
  retController->setPlayer(player);
  return retController;
}

} // namespace GEngine