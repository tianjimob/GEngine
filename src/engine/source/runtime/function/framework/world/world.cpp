#include "world.h"

#include "core/log/logger.h"

#include "_generated/serializer/all_serializer.h"

#include <memory>
#include <fstream>

namespace GEngine {

DECLARE_LOG_CATEGORY(LogWorld)

void World::init(const WorldInitializer &worldInitializer, GameInstance* gameInstance) {
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
    addLevel(level);
  }
  setCurrentLevel(worldInitializer.defautLevelUrl);
  m_owingGameInstance = gameInstance;
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

} // namespace GEngine