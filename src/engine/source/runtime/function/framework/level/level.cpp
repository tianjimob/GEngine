#include "level.h"

namespace GEngine {
void Level::tick(float deltaTime) {
  for (auto &actor : m_actors) {
    actor->tick(deltaTime);
  }
}

} // namespace GEngine