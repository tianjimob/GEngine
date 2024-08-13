#include "game_viewport_client.h"
#include "function/framework/engine/engine.h"
#include "function/framework/engine/scene_view.h"
#include "function/framework/render/renderer/renderer_module.h"
#include <memory>

namespace GEngine {
void GameViewportClient::createGameViewport() {
}

bool GameViewportClient::inputKey(InputEvent event, ModifierKey mod,
                                  VirtualCode key) {
  if (!GlobalEngine)
    return false;

  auto& players = GlobalEngine->getLocalPlayers();
  if (players.empty())
    return false;

  auto &player = players[0];
  if (player && player->getPlayerController()) {
    player->getPlayerController()->inputKey(event, mod, key);
    return true;
  } else {
    return false;
  }
  
}

void GameViewportClient::draw(SceneViewport *viewport) {
  std::shared_ptr<World> world;
  if (GlobalEngine) {
    world = GlobalEngine->getWorld();
  }
  SceneViewFamily viewFamily(world);
  for (auto &locaPlayer : GlobalEngine->getLocalPlayers()) {
    auto view = locaPlayer->calcSceneView(&viewFamily, viewport);
    viewFamily.addSceneView(view);
  }
  RendererModule::instance().renderViewFamily(viewFamily);
}

} // namespace GEngine