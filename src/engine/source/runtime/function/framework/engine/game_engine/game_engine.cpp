#include "game_engine.h"

#include <memory>

#include "core/chrono/time_guard.h"
#include "core/misc/config_cache_ini.h"
#include "function/framework/engine/game_viewport_client.h"
#include "function/framework/game_instance/game_instance.h"
#include "function/framework/render/core/rendering_thread.h"
#include "function/framework/render/rhi/rhi_shader.h"
#include "function/framework/render/rhi/vulkan/vulkan_rhi.h"
#include "resource/resource_path.h"


namespace GEngine {

static DECLARE_LOG_CATEGORY(LogGameEngine);

GameEngine::GameEngine() { GlobalEngine = this; }

void GameEngine::preInit(const std::string &configPath) {
  Engine::preInit(configPath);

  {
    // create gameinstance ...
    m_gameInstance = std::make_shared<GameInstance>();
    m_gameInstance->setEngine(
        std::static_pointer_cast<Engine>(shared_from_this()));
    m_gameInstance->preInit();
  }

  {
    std::shared_ptr<GameViewportClient> gameViewportClient;
    gameViewportClient = std::make_shared<GameViewportClient>();
    m_gameViewportClient = gameViewportClient;
  }

  {
    m_gameViewportWindow = createGameWindow();
    m_sceneViewport = std::make_shared<SceneViewport>();
    m_gameViewportClient->setViewport(m_sceneViewport);
    m_gameViewportWindow->setGameViewport(m_sceneViewport);
  }

  { m_gameInstance->createLocalPlayer(false); }

  // initialize RHI after created window
  {
    ConfigCacheIni &config = ConfigCacheIni::instance();
    auto getWindowSettingStr =
        [&config](const std::string &key,
                  const std::string &defaultValue) -> std::string {
      std::string res;
      res = config.getStr("Render", key, ResourcePath::gameIni);
      if (res.empty()) res = defaultValue;
      return res;
    };
    auto backend = getWindowSettingStr("rhi", "Vulkan");

    if (backend == "Vulkan") {
      GlobalRHI = new VulkanRHI;
    } else if (backend == "OpenGL45") {
      // GlobalRHI = new OpenGLRHI;
    }

    GlobalRHI->init();
    RHIShaders::init();
  }

  // start rendering thread
  {
    // startRenderingThread();
  }

  LOG_INFO(LogGameEngine, "Game Engine PreInitialized.");
}

void GameEngine::init() {
  LOG_INFO(LogGameEngine, "Initializing Game...");

  Engine::init();
  m_gameInstance->init(m_world);
  // m_gameInstance->getWorld()->setGameViewport(m_gameViewportClient);
}

void GameEngine::tick(float deltaTime) {
  SCOPE_TIME_GUARD(LogGameEngine);

  // m_gameViewportWindow->pollEvents();
  // m_shouldClose = m_gameViewportWindow->shouldClose();

  // m_gameInstance->tick(deltaTime);

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