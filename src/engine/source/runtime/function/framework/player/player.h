#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include <memory>

namespace GEngine {

CLASS(Player) : public GObject {

  REFLECTION_BODY(Player)
  
public:
  virtual void switchController(
      std::shared_ptr<PlayerController> playerController);
  std::shared_ptr<PlayerController> getPlayerController() const;
private:
  std::shared_ptr<PlayerController> m_playerController;
};

}