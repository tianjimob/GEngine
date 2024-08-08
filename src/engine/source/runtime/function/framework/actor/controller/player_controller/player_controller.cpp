#include "player_controller.h"
#include "function/framework/camera/player_camera_manager.h"
#include "function/framework/component/input/input_component.h"
#include "function/framework/player/player.h"
#include "function/framework/player/local_player/local_player.h"
#include "function/framework/actor/pawn/pawn.h"
#include "function/framework/input/player_input.h"
#include "function/framework/game_instance/game_instance.h"

#include <algorithm>
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
      if (auto actorComp = actorComponent.lock(); actorComp->isA<InputComponent>()) {
        auto inputComponent = std::static_pointer_cast<InputComponent>(actorComp);
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

void PlayerController::popInputComponent(InputComponent *inputComponent) {
  auto it = std::remove_if(
      m_currentInputStack.begin(), m_currentInputStack.end(),
      [inputComponent](const InputComponent *comp) { return inputComponent == comp; });
  m_currentInputStack.erase(it, m_currentInputStack.end());
}

void PlayerController::spwanPlayerCameraManager() {
  m_playerCameraManager =
      std::make_shared<PlayerCameraManager>(shared_from_this());
  m_playerCameraManager->setOuter(this);
  m_playerCameraManager->setViewTarget(
      std::static_pointer_cast<PlayerController>(shared_from_this()));
  m_playerCameraManager->updateCamera(0.f);
  
}

}  // namespace GEngine