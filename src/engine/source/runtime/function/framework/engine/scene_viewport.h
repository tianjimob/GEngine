#pragma once

#include "function/framework/ui/input.h"
#include <cstdint>
#include <memory>


namespace GEngine {

class Window;
class GameViewportClient;

class SceneViewport {
public:
  void setGameViewportClient(std::shared_ptr<GameViewportClient> client);

  bool onSizeChanged(Window *window, int32_t width, int32_t height,
                     bool wasMinimized) {
    return false;
  }
  bool onWindowClose(Window *window) { return false; }
  bool onKeyDown(ModifierKey mod, VirtualCode key, uint8_t repeat);
  bool onKeyUp(ModifierKey mod, VirtualCode key, uint8_t repeat);
  bool onMouseButtonUp(MouseButton button, double xpos, double ypos) {
    return false;
  }
  bool onMouseButtonDoubleClick(MouseButton button, double xpos, double ypos) {
    return false;
  }
  bool onMouseButtonDown(MouseButton button, double xpos, double ypos) {
    return false;
  }
  bool onMouseMove(double xpos, double ypos, double xrel, double yrel) {
    return false;
  }
  bool onMouseWheel(float delta, double xpos, double ypos) { return false; }

private:
  std::shared_ptr<GameViewportClient> m_gameViewportClient;
};

} // namespace GEngine