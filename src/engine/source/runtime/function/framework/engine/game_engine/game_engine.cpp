#include "game_engine.h"

#include <memory>

#include "core/chrono/time_guard.h"
#include "function/framework/engine/game_viewport_client.h"
#include "function/framework/game_instance/game_instance.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi.h"

namespace GEngine {
GameEngine::GameEngine() { GlobalEngine = this; }

DECLARE_LOG_CATEGORY(LogGameEngine);

void GameEngine::preInit(const std::string &configPath) {
  Engine::preInit(configPath);

  // todo: create gameinstance from deserializing
  {
    // create gameinstance ...
    m_gameInstance = std::make_shared<GameInstance>();
    m_gameInstance->setEngine(this);
    m_gameInstance->preInit();
  }

  // todo: GameViewportClient
  std::shared_ptr<GameViewportClient> gameViewportClient;
  gameViewportClient = std::make_shared<GameViewportClient>();
  m_gameViewportClient = gameViewportClient;

  {
    m_gameViewportWindow = createGameWindow();
    m_gameViewportWindow->setGameViewportClient(gameViewportClient);
  }

  // initialize RHI
  {
    GlobalRHI = new VulkanRHI;
    GlobalRHI->init();
  }

  LOG_INFO(LogGameEngine, "Game Engine PreInitialized.");
}

void GameEngine::init() {
  LOG_INFO(LogGameEngine, "Initializing Game...");

  Engine::init();
  m_gameInstance->init();
}

void GameEngine::tick(float deltaTime) {
  SCOPE_TIME_GUARD(LogGameEngine);

  m_gameViewportWindow->pollEvents();
  m_shouldClose = m_gameViewportWindow->shouldClose();

  m_gameInstance->tick(deltaTime);

  // todo: tick GameViewportClient
}

void GameEngine::exit() {
  LOG_INFO(LogGameEngine, "Exiting Game...");

  m_gameInstance->exit();

  m_gameViewportWindow->exit();

  Engine::exit();
}

std::shared_ptr<Window> GameEngine::createGameWindow() {
  std::shared_ptr<Window> ret = std::make_shared<Window>();
  ret->init();
  return ret;
}

}  // namespace GEngine