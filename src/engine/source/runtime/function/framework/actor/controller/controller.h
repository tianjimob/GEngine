#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"
#include <memory>

namespace GEngine {

class Pawn;

CLASS(Controller) : public Actor {

  REFLECTION_BODY(Controller)

public:
  // 
  virtual void posses(Pawn* pawn) = 0;
private:
    //   std::weak_ptr<PlayerState> m_playerState;
  std::weak_ptr<Pawn> m_controller;
};

}