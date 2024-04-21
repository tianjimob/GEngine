#include "local_player.h"

namespace GEngine {

bool LocalPlayer::spawnPlayActor(World *world) {
  if (!world)
    return false;

  m_playerController = world->spawnPlayActor(this);
}

}