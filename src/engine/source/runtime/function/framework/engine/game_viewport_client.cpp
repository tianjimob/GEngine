#include "game_viewport_client.h"
#include <memory>

namespace GEngine {
void GameViewportClient::createGameViewport() {
}

bool GameViewportClient::inputKey(InputEvent event, ModifierKey mod,
                                  VirtualCode key) {
  return true;
}

} // namespace GEngine