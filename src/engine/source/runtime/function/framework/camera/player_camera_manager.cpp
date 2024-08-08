#include "player_camera_manager.h"
#include "function/framework/actor/actor.h"
#include "function/framework/object/object.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include "function/framework/component/camera/camera_component.h"
#include <cassert>
#include <memory>

namespace GEngine {

PlayerCameraManager::PlayerCameraManager(
    std::shared_ptr<PlayerController> playerController) {}

void PlayerCameraManager::setViewTarget(std::shared_ptr<Actor> actor) {
  if (actor == nullptr) {
    if (auto owner = m_pcOwner.lock()) {
      actor = std::static_pointer_cast<Actor>(owner);
    }
  }

  if (actor == nullptr)
    return;

  auto targetActor = m_cameraViewTarget.actor.lock();
  if (targetActor && targetActor != actor) {
    
  }
  m_cameraViewTarget.actor = actor;
}

void PlayerCameraManager::updateCamera(float deltaTime) {
  auto pcOwner = m_pcOwner.lock();
  assert(pcOwner != nullptr);

  auto targetActor = m_cameraViewTarget.actor.lock();
  for (auto &component : targetActor->getComponents()) {
    auto comp = component.lock();
    if (comp->isA<CameraComponent>()) {
      m_cachedCameraComponent = std::static_pointer_cast<CameraComponent>(comp);
      break;
    }
  }

  auto cameraComp = m_cachedCameraComponent.lock();
  if (!cameraComp)
    return;

  cameraComp->getCameraView(deltaTime, m_cameraViewTarget.cameraViewInfo);
  
}
}