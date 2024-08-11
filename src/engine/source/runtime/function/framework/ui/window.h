#pragma once

#include "SDL2/SDL_video.h"
#include "SDL2/SDL_vulkan.h"
#include "core/log/logger.h"

#include "SDL2/SDL.h"
#include "core/math/ivector2.h"
#include "core/math/vector2.h"
#include <memory>
#include <string>

namespace GEngine {

enum class WindowBackend : uint8_t { OpenGL45, Vulkan };

struct WindowCreateInfo {
  WindowBackend backend;
  std::string title;
  int width;
  int height;
};

class SceneViewport;
class GameViewportClient;

class Window {
public:
  void init();
  void pollEvents();
  void exit();
  bool shouldClose() const;

  void setGameViewport(std::weak_ptr<SceneViewport> sceneViewport) {
    m_sceneViewport = sceneViewport;
  }

  void *getNativeHandle() { return m_sdlWindow; }

  IVector2 getDrawableSize() {
    int w,h;
    switch (m_backend) {
    case WindowBackend::OpenGL45:
      SDL_GL_GetDrawableSize(m_sdlWindow, &w, &h);
      break;
    case WindowBackend::Vulkan:
      SDL_Vulkan_GetDrawableSize(m_sdlWindow, &w, &h);
      break;
    default:
      break;
    }
    return {w, h};
  }

private:
  static const std::string defBackend;

  WindowBackend m_backend;
  SDL_Window *m_sdlWindow { nullptr };

  bool m_shouldClose{false};

  // pointer to SceneViewport created by GameInstance
  std::weak_ptr<SceneViewport> m_sceneViewport;

 private:
  SDL_Window *createOpenGL45Window(const WindowCreateInfo &createInfo);
  SDL_Window *createVulkanWindow(const WindowCreateInfo &createInfo);

  void handleWindowEvent(SDL_Event &event);
  void handleKeydownEvent(SDL_Event &event);
  void handleKeyupEvent(SDL_Event &event);
  void handleMouseButtonUpEvent(SDL_Event &event);
  void handleMouseButtonDoubleClick(SDL_Event &event);
  void handleMouseButtonDownEvent(SDL_Event &event);
  void handleMouseMotionEvent(SDL_Event &event);
  void handleMouseWheelEvent(SDL_Event &event);
  // void handleTouchDownEvent(SDL_TouchFingerEvent &event);
  
};

} // namespace GEngine