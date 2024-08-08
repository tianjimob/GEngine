#include "scene_viewport.h"

#include "function/framework/engine/engine.h"
#include "function/framework/engine/game_engine/game_engine.h"
#include "function/framework/engine/game_viewport_client.h"

namespace GEngine {
void SceneViewport::setGameViewportClient(GameViewportClient* client) {
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

void SceneViewport::draw(bool present) {
  if (GlobalEngine) {
    GameEngine *gameEngine = dynamic_cast<GameEngine*>(GlobalEngine);
    Window *window = (Window *)gameEngine->getWindow();
    if (window) {
      m_viewportRect.min = {0, 0};
      m_viewportRect.max = window->getDrawableSize();
    }
  }

  m_gameViewportClient->draw(this);
  
}

} // namespace GEngine