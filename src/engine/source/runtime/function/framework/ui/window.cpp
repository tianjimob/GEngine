#include "window.h"

#include "SDL2/SDL_vulkan.h"
#include "core/misc/config_cache_ini.h"
#include "function/framework/engine/scene_viewport.h"
#include "function/framework/ui/input.h"

#include <cstdint>
#include <memory>

namespace GEngine {

DECLARE_LOG_CATEGORY(LogWindow);

const std::string Window::defBackend = "Vulkan";

void Window::init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER |
               SDL_INIT_HAPTIC) != 0) {
    LOG_FATAL(LogWindow, "Can not initialize SDL : %s", SDL_GetError());
  }

  ConfigCacheIni &config = ConfigCacheIni::instance();
  auto getWindowSettingStr =
      [&config](const std::string &key,
                const std::string &defaultValue) -> std::string {
    std::string res;
    res = config.getStr("Window", key, "game.ini");
    if (res.empty())
      res = defaultValue;
    return res;
  };
  auto getWindowSettingInt = [&config](const std::string &key,
                                       int defaultValue) -> int32_t {
    int res = defaultValue;
    config.getInt("Window", key, "game.ini", res);
    return res;
  };

  WindowCreateInfo createInfo;
  createInfo.width = getWindowSettingInt("width", 1280);
  createInfo.height = getWindowSettingInt("height", 768);
  createInfo.title = getWindowSettingStr("title", "Game");

  auto backendStr = getWindowSettingStr("backend", defBackend);
  if (backendStr == "OpenGL45")
    m_backend = createInfo.backend = WindowBackend::OpenGL45;
  else if (backendStr == "Vulkan")
    m_backend = createInfo.backend = WindowBackend::Vulkan;

  switch (createInfo.backend) {
  case WindowBackend::OpenGL45:
    m_sdlWindow = createOpenGL45Window(createInfo);
    break;
  case WindowBackend::Vulkan:
    m_sdlWindow = createVulkanWindow(createInfo);
    break;
  }
}

void Window::pollEvents() {
  static SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      m_shouldClose = true;
      break;
    case SDL_WINDOWEVENT:
      handleWindowEvent(event);
      break;
      // Key
    case SDL_KEYDOWN:
      handleKeydownEvent(event);
      break;
    case SDL_KEYUP:
      handleKeyupEvent(event);
      break;
      // Mouse
    case SDL_MOUSEBUTTONUP:
      handleMouseButtonDownEvent(event);
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (event.button.clicks == 2)
        handleMouseButtonDoubleClick(event);
      else
        handleMouseButtonDownEvent(event);
      break;
    case SDL_MOUSEWHEEL:
      handleMouseWheelEvent(event);
      break;
      // Touch
    case SDL_FINGERDOWN:
      break;
    case SDL_FINGERMOTION:
      break;
    case SDL_FINGERUP:
      break;
      // Gesture
    case SDL_MULTIGESTURE:
      break;
    case SDL_DOLLARGESTURE:
      break;
    }
  }
}

void Window::exit() {
  SDL_DestroyWindow(m_sdlWindow);
  SDL_Quit();
}

bool Window::shouldClose() const { return m_shouldClose; }

SDL_Window *Window::createOpenGL45Window(const WindowCreateInfo &createInfo) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags windowFlags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  m_sdlWindow = SDL_CreateWindow(
      createInfo.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      createInfo.width, createInfo.height, windowFlags);
  if (m_sdlWindow == nullptr) {
    LOG_FATAL(LogWindow, "Can not create window : %s", SDL_GetError());
  }

  return m_sdlWindow;
}

SDL_Window *Window::createVulkanWindow(const WindowCreateInfo &createInfo) {
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
  SDL_WindowFlags windowFlags =
      (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  m_sdlWindow = SDL_CreateWindow(
      createInfo.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      createInfo.width, createInfo.height, windowFlags);
  if (m_sdlWindow == nullptr) {
    LOG_FATAL(LogWindow, "Can not create window : %s", SDL_GetError());
  }
  return m_sdlWindow;
}

void Window::handleWindowEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;
  switch (event.window.event) {
  case SDL_WINDOWEVENT_SHOWN:
    break;
  case SDL_WINDOWEVENT_HIDDEN:
    break;
  case SDL_WINDOWEVENT_EXPOSED:
    break;
  case SDL_WINDOWEVENT_MOVED:
    break;
  case SDL_WINDOWEVENT_RESIZED:
  case SDL_WINDOWEVENT_SIZE_CHANGED: {
    auto flags = SDL_GetWindowFlags(m_sdlWindow);
    if (!(flags & SDL_WINDOW_FULLSCREEN)) {
      if(auto viewport = m_sceneViewport.lock())
        viewport->onSizeChanged(this, event.window.data1, event.window.data2,
                                flags & SDL_WINDOW_MINIMIZED);
    }
  } break;
  case SDL_WINDOWEVENT_MINIMIZED:
    break;
  case SDL_WINDOWEVENT_MAXIMIZED:
    break;
  case SDL_WINDOWEVENT_RESTORED:
    break;
  case SDL_WINDOWEVENT_ENTER:
    break;
  case SDL_WINDOWEVENT_LEAVE:
    break;
  case SDL_WINDOWEVENT_FOCUS_GAINED:
    break;
  case SDL_WINDOWEVENT_FOCUS_LOST:
    break;
  case SDL_WINDOWEVENT_CLOSE:
    if(auto viewport = m_sceneViewport.lock())
        viewport->onWindowClose(this);
    m_shouldClose = true;
    break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
  case SDL_WINDOWEVENT_TAKE_FOCUS:
    break;
  case SDL_WINDOWEVENT_HIT_TEST:
    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 18)
  case SDL_WINDOWEVENT_DISPLAY_CHANGED:
    break;
  case SDL_WINDOWEVENT_ICCPROF_CHANGED:
    break;
#endif
  default:
    break;
  }
}

void Window::handleKeydownEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  ModifierKey mod;
  switch (event.key.keysym.mod) {
  case KMOD_LSHIFT:
    mod = ModifierKey::LeftShift;
    break;
  case KMOD_RSHIFT:
    mod = ModifierKey::RightShift;
    break;
  case KMOD_LCTRL:
    mod = ModifierKey::LeftCtrl;
    break;
  case KMOD_RCTRL:
    mod = ModifierKey::RightCtrl;
    break;
  case KMOD_LALT:
    mod = ModifierKey::LeftAlt;
    break;
  case KMOD_RALT:
    mod = ModifierKey::RightShift;
    break;
  case KMOD_LGUI:
    mod = ModifierKey::LeftGui;
    break;
  case KMOD_RGUI:
    mod = ModifierKey::RightGui;
    break;
  case KMOD_NUM:
    mod = ModifierKey::NumLock;
    break;
  case KMOD_CAPS:
    mod = ModifierKey::CapsLock;
    break;
  case KMOD_SHIFT:
    mod = ModifierKey::Shift;
    break;
  case KMOD_CTRL:
    mod = ModifierKey::Ctrl;
    break;
  case KMOD_ALT:
    mod = ModifierKey::Alt;
    break;
  case KMOD_GUI:
    mod = ModifierKey::Gui;
    break;
  }

  if(auto viewport = m_sceneViewport.lock())
        viewport->onKeyDown(
      mod, static_cast<VirtualCode>(event.key.keysym.sym), event.key.repeat);
}

void Window::handleKeyupEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  ModifierKey mod;
  switch (event.key.keysym.mod) {
  case KMOD_LSHIFT:
    mod = ModifierKey::LeftShift;
    break;
  case KMOD_RSHIFT:
    mod = ModifierKey::RightShift;
    break;
  case KMOD_LCTRL:
    mod = ModifierKey::LeftCtrl;
    break;
  case KMOD_RCTRL:
    mod = ModifierKey::RightCtrl;
    break;
  case KMOD_LALT:
    mod = ModifierKey::LeftAlt;
    break;
  case KMOD_RALT:
    mod = ModifierKey::RightShift;
    break;
  case KMOD_LGUI:
    mod = ModifierKey::LeftGui;
    break;
  case KMOD_RGUI:
    mod = ModifierKey::RightGui;
    break;
  case KMOD_NUM:
    mod = ModifierKey::NumLock;
    break;
  case KMOD_CAPS:
    mod = ModifierKey::CapsLock;
    break;
  case KMOD_SHIFT:
    mod = ModifierKey::Shift;
    break;
  case KMOD_CTRL:
    mod = ModifierKey::Ctrl;
    break;
  case KMOD_ALT:
    mod = ModifierKey::Alt;
    break;
  case KMOD_GUI:
    mod = ModifierKey::Gui;
    break;
  }

  if(auto viewport = m_sceneViewport.lock())
        viewport->onKeyUp(mod, static_cast<VirtualCode>(event.key.keysym.sym),
                           event.key.repeat);
}

void Window::handleMouseButtonUpEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  MouseButton button;
  switch (event.button.button) {
  case SDL_BUTTON_LEFT:
    button = MouseButton::Left;
    break;
  case SDL_BUTTON_MIDDLE:
    button = MouseButton::Middle;
    break;
  case SDL_BUTTON_RIGHT:
    button = MouseButton::Right;
    break;
  case SDL_BUTTON_X1:
    button = MouseButton::X1;
    break;
  case SDL_BUTTON_X2:
    button = MouseButton::X2;
    break;
  default:
    break;
  }
  if(auto viewport = m_sceneViewport.lock())
        viewport->onMouseButtonUp(button, event.button.x, event.button.y);
}

void Window::handleMouseButtonDoubleClick(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  MouseButton button;
  switch (event.button.button) {
  case SDL_BUTTON_LEFT:
    button = MouseButton::Left;
    break;
  case SDL_BUTTON_MIDDLE:
    button = MouseButton::Middle;
    break;
  case SDL_BUTTON_RIGHT:
    button = MouseButton::Right;
    break;
  case SDL_BUTTON_X1:
    button = MouseButton::X1;
    break;
  case SDL_BUTTON_X2:
    button = MouseButton::X2;
    break;
  default:
    break;
  }
  if(auto viewport = m_sceneViewport.lock())
        viewport->onMouseButtonDoubleClick(button, event.button.x,
                                            event.button.y);
}

void Window::handleMouseButtonDownEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  MouseButton button;
  switch (event.button.button) {
  case SDL_BUTTON_LEFT:
    button = MouseButton::Left;
    break;
  case SDL_BUTTON_MIDDLE:
    button = MouseButton::Middle;
    break;
  case SDL_BUTTON_RIGHT:
    button = MouseButton::Right;
    break;
  case SDL_BUTTON_X1:
    button = MouseButton::X1;
    break;
  case SDL_BUTTON_X2:
    button = MouseButton::X2;
    break;
  default:
    break;
  }
  if(auto viewport = m_sceneViewport.lock())
        viewport->onMouseButtonDown(button, event.button.x, event.button.y);
}

void Window::handleMouseMotionEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  MouseButton button;
  switch (event.button.button) {
  case SDL_BUTTON_LEFT:
    button = MouseButton::Left;
    break;
  case SDL_BUTTON_MIDDLE:
    button = MouseButton::Middle;
    break;
  case SDL_BUTTON_RIGHT:
    button = MouseButton::Right;
    break;
  case SDL_BUTTON_X1:
    button = MouseButton::X1;
    break;
  case SDL_BUTTON_X2:
    button = MouseButton::X2;
    break;
  default:
    break;
  }
  if(auto viewport = m_sceneViewport.lock())
        viewport->onMouseMove(event.motion.x, event.motion.y,
                               event.motion.xrel, event.motion.yrel);
}

void Window::handleMouseWheelEvent(SDL_Event &event) {
  bool isThisWindow = (event.window.windowID == SDL_GetWindowID(m_sdlWindow));
  if (!isThisWindow)
    return;

  const float spinFactor = 1 / 120.f;
#if SDL_VERSION_ATLEAST(2, 0, 26)
  if(auto viewport = m_sceneViewport.lock())
        viewport->onMouseWheel(event.wheel.preciseY * spinFactor,
                                event.wheel.mouseX, event.wheel.mouseY);
#elif SDL_VERSION_ATLEAST(2, 0, 18)
  int32_t x, y;
  SDL_GetMouseState(&x, &y);
  m_sceneViewport->onMouseWheel(event.wheel.preciseY * spinFactor, x, y);
#else
  int32_t x, y;
  SDL_GetMouseState(&x, &y);
  m_sceneViewport->onMouseWheel(static_cast<float>(event.wheel.y) * spinFactor,
                                x, y);
#endif
}

} // namespace GEngine