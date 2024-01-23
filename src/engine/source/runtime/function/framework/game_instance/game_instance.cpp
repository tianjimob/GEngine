#include "game_instance.h"
#include "function/framework/engine/engine.h"
#include <memory>

namespace GEngine {
void GameInstance::preInit() {}

void GameInstance::init() {
  //   m_world = m_engine->loadWorld();
  m_world = std::make_shared<World>();
}

void GameInstance::tick(float deltaTime) { m_world->tick(deltaTime); }

void GameInstance::exit() {}

void GameInstance::setEngine(Engine *engine) { m_engine = engine; }

} // namespace GEngine