#include "scene_viewport.h"

#include "function/framework/engine/game_viewport_client.h"

namespace GEngine {
void SceneViewport::setGameViewportClient(
    std::shared_ptr<GameViewportClient> client) {
  m_gameViewportClient = client;
}

bool SceneViewport::onKeyDown(ModifierKey mod, VirtualCode key, uint8_t repeat)
{
    return m_gameViewportClient->inputKey(repeat >= 2? InputEvent::DoubleClick : InputEvent::Pressed, mod, key);
}

bool SceneViewport::onKeyUp(ModifierKey mod, VirtualCode key, uint8_t repeat)
{
    return m_gameViewportClient->inputKey(repeat >= 2? InputEvent::DoubleClick : InputEvent::Released, mod, key);
}

} // namespace GEngine