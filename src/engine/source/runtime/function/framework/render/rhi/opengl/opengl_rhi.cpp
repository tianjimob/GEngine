#include "opengl_rhi.h"

#include "core/log/logger.h"
#include "function/framework/engine/engine.h"
#include "function/framework/engine/game_engine/game_engine.h"
#include "glad/glad.h"

namespace GEngine {

DECLARE_LOG_CATEGORY(LogOpenGLRHI);

OpenGLRHI::OpenGLRHI() {
  instance = this;

  createContext();
  
}

void OpenGLRHI::createContext()
{
  SDL_Window *sdlWindow = nullptr;
  if (GlobalEngine) {
    GameEngine *gameEngine = (GameEngine *)GlobalEngine;
    Window *window = (Window *)gameEngine->getWindow();
    if (window) {
      sdlWindow = (SDL_Window *)window->getNativeHandle();
    }
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
  SDL_GL_MakeCurrent(sdlWindow, glContext);
  if (SDL_GL_SetSwapInterval(-1) == -1) {
    SDL_GL_SetSwapInterval(1);
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    LOG_FATAL(LogOpenGLRHI, "Glad failed load OpenGL library");
  }
}

} // namespace GEngine