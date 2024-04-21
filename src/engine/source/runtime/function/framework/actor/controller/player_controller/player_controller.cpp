#include "player_controller.h"
#include "function/framework/player/player.h"
#include "function/framework/player/local_player/local_player.h"
#include "function/framework/actor/pawn/pawn.h"
#include "function/framework/input/player_input.h"
#include "function/framework/game_instance/game_instance.h"

#include <memory>
#include <vector>
namespace GEngine {

PlayerController *PlayerController::GetPlayControllerFromActor(Actor *actor) {
  if(!actor) return nullptr;

  auto level = actor->getLevel().lock();
  if (!level)
    return nullptr;

  auto world = level->getWorld().lock();
  if (!world)
    return nullptr;

  auto gameInstance = world->getGameInstance().lock();
  if (!gameInstance)
    return nullptr;

  for (auto &localPlayer : gameInstance->getLocalPlayers()) {
    if (auto playerController = localPlayer->getPlayerController()) {
      return playerController.get();
    }
  }

  return nullptr;
}

void PlayerController::setPlayer(Player *player) {
  m_player = player;
  m_player->setPlayerController(this);
  if (m_player->isA<LocalPlayer>()) {
    initInputSystem();
  }
}

void PlayerController::tick(float deltaTime) {
  static std::vector<InputComponent *> inputStack;

  if (auto pawn = m_pawn.lock(); pawn) {

    if (auto inputComponent = pawn->getInputComponent(); inputComponent) {
      inputStack.push_back(inputComponent.get());
    }

    for (auto &actorComponent : pawn->getComponents()) {
      if (actorComponent->isA<InputComponent>()) {
        auto inputComponent = std::static_pointer_cast<InputComponent>(actorComponent);
        if (inputComponent && inputComponent != pawn->getInputComponent()) {
          inputStack.emplace_back(inputComponent.get());
        }
      }
    }
  }

  if (auto inputComponent = getInputComponent(); inputComponent) {
    inputStack.emplace_back(inputComponent.get());
  }

  for (auto *inputComponent : m_currentInputStack) {
    inputStack.emplace_back(inputComponent);
  }

  m_playerInput->tickInputStack(inputStack, deltaTime);

  inputStack.clear();
}

void PlayerController::initInputSystem() {
  m_playerInput = std::make_shared<PlayerInput>();
  m_inputComponent = std::make_shared<InputComponent>();
}

void PlayerController::pushInputComponent(InputComponent *inputComponent) {
  m_currentInputStack.emplace_back(inputComponent);
}

}  // namespace GEngine