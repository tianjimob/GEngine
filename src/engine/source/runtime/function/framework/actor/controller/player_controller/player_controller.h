#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"
#include "function/framework/actor/controller/controller.h"
#include "function/framework/actor/pawn/pawn.h"
#include "function/framework/component/input/input_component.h"
#include <memory>
#include <vector>

namespace GEngine {

class Player;
class PlayerInput;
class World;

CLASS(PlayerController) : public Controller {

  REFLECTION_BODY(PlayerController)

public:
  static PlayerController *GetPlayControllerFromActor(Actor * actor);
  
  virtual void setPlayer(Player * player);
  
  virtual void tick(float deltaTime) override;

  virtual void pushInputComponent(InputComponent * inputComponent);

  virtual void popInputComponent(InputComponent * inputComponent);

  void setWorld(std::weak_ptr<World> world) { m_world = world; }
  void setPawn(std::weak_ptr<Pawn> pawn) { m_pawn = pawn; }

private:
  // bind to player
  Player *m_player;

  // pointer world created this controller
  std::weak_ptr<World> m_world;
  // pointer to pwan controlled by this controller
  std::weak_ptr<Pawn> m_pawn;

  std::shared_ptr<PlayerInput> m_playerInput;

  std::vector<InputComponent*> m_currentInputStack;

private:
  void initInputSystem();
};

} // namespace GEngine