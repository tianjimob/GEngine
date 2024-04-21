#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include <memory>

namespace GEngine {

CLASS(Player) : public GObject {

  REFLECTION_BODY(Player)
  
public:
  void setPlayerController(PlayerController * playerController);
  
private:
  PlayerController* m_playerController;
};

}