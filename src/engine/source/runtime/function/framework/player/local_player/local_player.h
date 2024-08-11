#pragma once

#include "core/math/matrix4.h"
#include "core/math/rect.h"
#include "core/math/vector2.h"
#include "core/reflection/reflection.h"
#include "function/framework/actor/controller/player_controller/player_controller.h"
#include "function/framework/camera/player_camera_manager.h"
#include "function/framework/engine/scene_view.h"
#include "function/framework/engine/scene_viewport.h"
#include "function/framework/player/player.h"
#include "function/framework/world/world.h"
#include <memory>

namespace GEngine {

class GameViewportClient;

CLASS(LocalPlayer) : public Player {

  REFLECTION_BODY(LocalPlayer)

public:
  bool spawnPlayActor(World * world);
  std::shared_ptr<PlayerController> &getPlayerController() { return m_playerController; }
  std::shared_ptr<SceneView> calcSceneView(SceneViewport* viewport);

private:
  std::shared_ptr<PlayerController> m_playerController;
  GameViewportClient *m_gameViewportClient;

  // left up position, [0, 1]
  Vector2 origin;

  // [0, 1]
  Vector2 extent;

private:
  Rect calcViewRect(SceneViewport* viewport);
  Rect calcConstrainedViewRect(float aspectRatio, const Rect& viewRect);
  void calcProjectionMatrixFromViewInfo(const CameraViewInfo& viewInfo,
                                        const Rect& viewRect, float near,
                                        float far, Matrix4x4& projectionMatrix);
};

}