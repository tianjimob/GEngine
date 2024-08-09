#include "local_player.h"
#include "function/framework/engine/scene_view.h"
#include <memory>
namespace GEngine {

bool LocalPlayer::spawnPlayActor(World *world) {
  if (!world)
    return false;

  m_playerController = world->spawnPlayActor(this);
  m_playerController->spwanPlayerCameraManager();

  return true;
}

std::shared_ptr<SceneView> LocalPlayer::calcSceneView() {
  SceneViewInitOptions initOptions;
  std::shared_ptr<SceneView> sceneView =
      std::make_shared<SceneView>(initOptions);
  return sceneView;
}

}