#include "engine.h"

#include "core/reflection/reflection.h"

namespace GEngine {

Engine* GlobalEngine = nullptr;

DECLARE_LOG_CATEGORY(LogEngine);

void Engine::preInit(const std::string &configPath) {
  LOG_INFO(LogEngine, "PreInitializeing Engine...");
  struct ScopeEnginePreInitGuard {
    ~ScopeEnginePreInitGuard() {
      LOG_INFO(LogEngine, "Engine PreInitialized.");
    };
  } scopeGuard;

  Reflection::autoRegisterAll();
}

void Engine::exit() {}

void Engine::init() {}

} // namespace GEngine