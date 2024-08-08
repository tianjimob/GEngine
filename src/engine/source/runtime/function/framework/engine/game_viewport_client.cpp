#include "game_viewport_client.h"
#include "function/framework/engine/engine.h"
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

  
  return true;
}

void GameViewportClient::draw(SceneViewport* viewport) {
  
}

} // namespace GEngine