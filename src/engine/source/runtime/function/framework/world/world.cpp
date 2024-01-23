#include "world.h"
#include <memory>

namespace GEngine {

void World::tick(float deltaTime) {
  std::shared_ptr<Level> level = m_currentLevel.lock();
  if (level) {
    level->tick(deltaTime);
  }
}

} // namespace GEngine