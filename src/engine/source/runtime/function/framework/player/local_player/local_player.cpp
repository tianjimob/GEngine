#include "local_player.h"
#include "core/math/color.h"
#include "core/math/ivector4.h"
#include "core/math/math.h"
#include "function/framework/camera/player_camera_manager.h"
#include "function/framework/engine/scene_view.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <memory>
namespace GEngine {

bool LocalPlayer::spawnPlayActor(World *world) {
  if (!world)
    return false;

  m_playerController = world->spawnPlayActor(this);
  m_playerController->spwanPlayerCameraManager();

  return true;
}

std::shared_ptr<SceneView> LocalPlayer::calcSceneView(SceneViewport* viewport) {
  const CameraViewInfo& viewInfo =
      m_playerController->getPlayerCameraManager()->getCameraViewInfo();
  SceneViewInitOptions initOptions;
  initOptions.viewActor = m_playerController->getPlayerCameraManager()->getCameraViewActor();
  initOptions.unconstrainedViewRect = calcViewRect(viewport);
  initOptions.constrainedViewRect = calcConstrainedViewRect(
      viewInfo.aspectRatio, initOptions.unconstrainedViewRect);
  initOptions.viewRotationMatrix = viewInfo.rotator.getInverseRotatorMatrix();
  calcProjectionMatrixFromViewInfo(
      viewInfo, initOptions.unconstrainedViewRect, viewport->getNearClipPlane(),
      viewport->getFarClipPlane(), initOptions.projectionMatrix);
  initOptions.viewOrigin = viewInfo.location;
  initOptions.viewLocation = viewInfo.location;
  initOptions.viewRotation = viewInfo.rotator;
  initOptions.fovx = viewInfo.fov;
  initOptions.backgroundColor = LinearColor::Black;

  std::shared_ptr<SceneView> sceneView =
      std::make_shared<SceneView>(initOptions);
  return sceneView;
}

Rect LocalPlayer::calcViewRect(SceneViewport* viewport) {
  Rect ret;
  IVector4 iv4;
  Vector4 fv4{
      origin.x * viewport->getSize().x, origin.y * viewport->getSize().y,
      extent.x * viewport->getSize().x, extent.y * viewport->getSize().y};
  Math::truncToInt32(&iv4, &fv4);
  return {{iv4.x, iv4.y}, {iv4.z, iv4.w}};
}

Rect LocalPlayer::calcConstrainedViewRect(float aspectRatio,
                                          const Rect &viewRect) {
  Rect result = viewRect;
  const float width = viewRect.width();
  const float height = viewRect.height();
  const float aspectDiff = aspectRatio - (width / height);
  if (std::abs(aspectDiff) > 0.01f) {
    // If desired aspect ratio is bigger than current - we need black bars on top and bottom.
    if (aspectDiff > 0.f) {
      // aspectRatio = width / height --> height = width / aspectRatio
      const int32_t newHeight = std::round(width/aspectRatio);
      result.min.y += std::round(0.5f * (height - newHeight));
      result.max.y = result.min.y + newHeight;
    }
    // Otherwise - will place bars on the sides.
    else {
      // aspectRatio = width / height --> width = height * aspectRatio
      const int32_t newWidth = std::round(height * aspectRatio);
      result.min.x += std::round(0.5f * (width - newWidth));
      result.max.x = result.min.x + newWidth;
    }
  }
  return result;
}

void LocalPlayer::calcProjectionMatrixFromViewInfo(
    const CameraViewInfo &viewInfo, const Rect &viewRect,float near,
                                        float far, 
    Matrix4x4 &projectionMatrix) {
  const float width = viewRect.width();
  const float height = viewRect.height();
  float aspect =
      viewInfo.isConstrainAspectRatio ? viewInfo.aspectRatio : (width / height);
  const float hfovy = std::atanf(
      std::tan(Degree{std::max(0.001f, viewInfo.fov) / 2.f}.valueRadians()) /
      aspect);
  projectionMatrix = Matrix4x4::ZERO;
  const float tanHfovy = std::tan(hfovy);
  projectionMatrix[0][0] = 1.f / (aspect * tanHfovy);
  projectionMatrix[1][1] = 1 / tanHfovy;
  projectionMatrix[2][2] = far / (near - far);
  projectionMatrix[2][3] = (far * near) / (near - far);
  projectionMatrix[3][2] = -1.f;
}

}