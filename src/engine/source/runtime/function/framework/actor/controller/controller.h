#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"
#include <memory>

namespace GEngine {

class Pawn;

CLASS(Controller) : public Actor {

  REFLECTION_BODY(Controller)

public:
  void process(std::shared_ptr<Pawn> pawn);
  void unprocess();
private:
    //   std::weak_ptr<PlayerState> m_playerState;
  std::weak_ptr<Pawn> m_controller;
};

}