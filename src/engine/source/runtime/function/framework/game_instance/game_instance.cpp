#include "game_instance.h"

#include <fstream>
#include <memory>

#include "function/framework/engine/engine.h"
#include "core/misc/config_cache_ini.h"
#include "resource/resource_path.h"
#include "core/log/logger.h"

#include "_generated/serializer/all_serializer.h"


namespace GEngine {
DECLARE_LOG_CATEGORY(LogGameInstance)

void GameInstance::preInit() {}

void GameInstance::init() {
  m_world = std::make_shared<World>();

  std::string worldJsonPath;
  auto success = ConfigCacheIni::instance().getString("World", "world", ResourcePath::projectIni, worldJsonPath);
  if (!success)
    LOG_FATAL(LogGameInstance, "failed to find world json at {}:{}", __FILE__,
              __LINE__);

  WorldInitializer worldInitializer;
  {  
    std::ifstream worldJson{worldJsonPath};
    if (!worldJson)
      LOG_FATAL(LogGameInstance, "failed to load file {} at {}:{}",
                worldJsonPath, __FILE__, __LINE__);
    Json data = Json::parse(worldJson);
    Serializer::read(data, worldInitializer);
  }
  m_world->load(worldInitializer, std::static_pointer_cast<GameInstance>(shared_from_this()));

  for (auto &localPlayer : m_localPlayers) {
    localPlayer->spawnPlayActor(m_world.get());
  }

}

void GameInstance::tick(float deltaTime) {
  for (auto &localPlayer : m_localPlayers) {
    localPlayer->getPlayerController()->tick(deltaTime);
  }
  m_world->tick(deltaTime);
}

void GameInstance::exit() {}

void GameInstance::setEngine(std::weak_ptr<Engine> engine) { m_engine = engine; }

std::shared_ptr<LocalPlayer> &GameInstance::createLocalPlayer(bool shouldSpawnPlayActor) {
  auto &newPlayer = m_localPlayers.emplace_back();
  if (shouldSpawnPlayActor && newPlayer && m_world) {
    if (!newPlayer->spawnPlayActor(m_world.get())) {
      m_localPlayers.pop_back();
      newPlayer.reset();
    }
  }
  return newPlayer;
}

}  // namespace GEngine