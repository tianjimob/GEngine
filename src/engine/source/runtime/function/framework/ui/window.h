#pragma once

#include "core/log/logger.h"

#include "SDL2/SDL.h"
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

  void setGameViewport(SceneViewport *sceneViewport) {
    m_sceneViewport = sceneViewport;
  }

  void* getNativeHandle() { return m_sdlWindow; }

private:
  static const std::string defBackend;
  
  SDL_Window *m_sdlWindow { nullptr };

  bool m_shouldClose{false};

  // pointer to SceneViewport created by GameInstance
  SceneViewport* m_sceneViewport;

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