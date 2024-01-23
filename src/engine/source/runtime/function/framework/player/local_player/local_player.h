#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include "function/framework/player/player.h"
#include "function/framework/world/world.h"
#include <memory>

namespace GEngine {

CLASS(LocalPlayer) : public Player {
public:
  std::shared_ptr<World> getWorld();
private:
  // std::shared_ptr<GameViewportClient> m_gameViewportClient;
};

}