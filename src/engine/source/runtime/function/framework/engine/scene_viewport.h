#pragma once

#include "core/math/rect.h"
#include "core/math/vector2.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include "function/framework/ui/input.h"
#include "function/framework/render/core/color_attachment.h"
#include <cstdint>
#include <memory>


namespace GEngine {

class Window;
class GameViewportClient;

class SceneViewport {
public:
  void setGameViewportClient(GameViewportClient* client);
  void initViewport();

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

  void draw(bool present);

  IVector2 getSize() { return m_viewportRect.max - m_viewportRect.min; }

  float getNearClipPlane() { return 10.f; }
  float getFarClipPlane() { return 1000.f; }

private:
  // pointer to GameViewportClient
  GameViewportClient *m_gameViewportClient;

  // range in pixels
  Rect m_viewportRect;

  std::shared_ptr<RHIViewport> m_rhiViewport;
};

} // namespace GEngine