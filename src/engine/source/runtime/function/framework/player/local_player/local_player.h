#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include "function/framework/engine/scene_view.h"
#include "function/framework/player/player.h"
#include "function/framework/world/world.h"
#include <memory>

namespace GEngine {

class GameViewportClient;

CLASS(LocalPlayer) : public Player {

  REFLECTION_BODY(LocalPlayer)

public:
  bool spawnPlayActor(World * world);
  auto &getPlayerController() { return m_playerController; }
  std::shared_ptr<SceneView> calcSceneView();

private:
  std::shared_ptr<PlayerController> m_playerController;
  GameViewportClient* m_gameViewportClient;
};

}