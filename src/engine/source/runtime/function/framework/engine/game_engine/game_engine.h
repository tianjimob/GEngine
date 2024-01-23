#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/engine/engine.h"
#include "core/log/logger.h"
#include "function/framework/ui/window.h"
#include <memory>

namespace GEngine {

CLASS(GameEngine): public Engine {

  REFLECTION_BODY(GameEngine)

public:
  GameEngine();
  
  virtual void preInit(const std::string &configPath) override;
  virtual void init() override;
  virtual void tick(float deltaTime) override;
  virtual void exit() override;

  Window* getWindow() { return m_gameViewportWindow.get(); }

private:
  std::shared_ptr<GameInstance> m_gameInstance;
  std::shared_ptr<Window> m_gameViewportWindow;

private:
  
  static std::shared_ptr<Window> createGameWindow();
};

}