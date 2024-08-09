#pragma once

#include "core/math/rotator.h"
#include "core/math/vector3.h"
#include "function/framework/actor/actor.h"
#include "function/framework/camera/camera_types.h"
#include <memory>
namespace GEngine {

struct CameraViewInfo {
  Vector3 location;
  Rotator rotator;
  float fov;
  float aspectRatio;
  float postProcessBlendWeight;
  PostProcessSetting postProcessSetting;
};

struct CameraViewTarget {
  std::weak_ptr<Actor> actor;
  CameraViewInfo cameraViewInfo;
};

class CameraComponent;

class PlayerCameraManager {
public:
  PlayerCameraManager(std::shared_ptr<PlayerController> playerController);

  void setViewTarget(std::shared_ptr<Actor> actor);
  void updateCamera(float deltaTime);

private:
  std::weak_ptr<PlayerController> m_pcOwner;
  CameraViewTarget m_cameraViewTarget;
  std::weak_ptr<CameraComponent> m_cachedCameraComponent;

};

}