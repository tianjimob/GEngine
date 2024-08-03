#include "level.h"

#include "function/framework/world/world.h"

namespace GEngine {
void Level::tick(float deltaTime) {
  for (auto &actor : m_actors) {
    actor->tick(deltaTime);
  }
}

void Level::postLoad(std::weak_ptr<GObject> parentObject) {
  if (auto object = parentObject.lock(); object->isA<World>()) {
    if (getOuter() != object.get()) {
      setOuter(object.get());
    }
    m_world = std::static_pointer_cast<World>(object);
  }
}

} // namespace GEngine