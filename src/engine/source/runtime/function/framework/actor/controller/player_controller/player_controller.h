#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/controller/controller.h"
#include <memory>
#include <vector>

namespace GEngine {

class Player;

CLASS(PlayerController) : public Controller {

  REFLECTION_BODY(PlayerController)

public:
  void setPlayer(Player* player);

private:
  Player* m_player;
  //   std::shared_ptr<HUD> m_hud;
  //   std::vector<std::shared_ptr<CameraComponent>> m_cameras;
};

} // namespace GEngine