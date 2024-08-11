#include "game_viewport_client.h"
#include "function/framework/engine/engine.h"
#include "function/framework/engine/scene_view.h"
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

void GameViewportClient::draw(SceneViewport* viewport) {
  SceneViewFamily viewFamily;
  for (auto &locaPlayer : GlobalEngine->getLocalPlayers()) {
    viewFamily.addSceneView(locaPlayer->calcSceneView(viewport));
  }
}

} // namespace GEngine