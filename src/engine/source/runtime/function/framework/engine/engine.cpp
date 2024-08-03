#include "engine.h"

#include "core/log/logger.h"
#include "core/misc/util_string.h"
#include "core/reflection/reflection.h"
#include "function/framework/input/key.h"
#include "core/misc/config_cache_ini.h"
#include "function/framework/world/world.h"
#include "resource/resource_path.h"
#include "core/misc/json.h"
#include "core/serializer/serializer.h"
#include "function/framework/input/input_settings.h"
#include "function/framework/game_instance/game_instance.h"
#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace GEngine {

Engine* GlobalEngine = nullptr;

DECLARE_LOG_CATEGORY(LogEngine);

template <typename Src, typename Dst> bool dynamicTest(Src *src) {
  Dst *dst = dynamic_cast<Dst *>(src);
  if (dst)
    return true;
  else
   return false;
}

void Engine::preInit(const std::string &configPath) {
  LOG_INFO(LogEngine, "PreInitializeing Engine...");
  struct ScopeEnginePreInitGuard {
    ~ScopeEnginePreInitGuard() {
      LOG_INFO(LogEngine, "Engine PreInitialized.");
    };
  } scopeGuard;

  // register classes into Registry::instance()
  Reflection::autoRegisterAll();
  // now ClassDescriptor::m_superClass is nullptr so we can get super class
  // name (ClassDescriptor::getSuperClassName()) and set m_superClass as
  // ClassDescriptor with super class name
  Reflection::autoSetupSuperClassInfo();
  // everything about reflection infos was setup

  // init input system and register axis/action mappings to InputSetting::instance
  initInputSystem();
}

void Engine::init() { m_world = std::make_shared<World>(); }

void Engine::exit() {}

std::vector<std::shared_ptr<LocalPlayer>> &Engine::getLocalPlayers() {
    std::shared_ptr<GameInstance> gameInstance;
    if (m_world) {
      gameInstance = m_world->getGameInstance().lock();
    }

    if (gameInstance) return gameInstance->getLocalPlayers();
}

void Engine::initInputSystem() {
  Keys::init();

  Json inputsJson;
  try {
    std::ifstream is(ResourcePath::inputsJson);
    if (!is.is_open()) {
      LOG_ERROR(LogEngine, "Failed load file %s", ResourcePath::inputsJson);
    }
    inputsJson = Json::parse(is);

    assert(inputsJson.is_object());
  } catch (Json::exception& e) {
    LOG_ERROR(LogEngine, "Failed parse %s: %s", ResourcePath::inputsJson,
              e.what());
    return;
  }

  {
    assert(inputsJson["AxisMappings"].is_array());

    std::for_each(inputsJson["AxisMappings"].begin(),
                  inputsJson["AxisMappings"].end(), [this](const Json &json) {
                    InputAxisKeyMappingInitializer initializer;
                    Serializer::read(json, initializer, this);
                    InputAxisKeyMapping mapping{initializer};
                    mapping.postLoad({});
                    InputSettings::instance().addAxisKeyMapping(mapping);
                  });
  }

  {
    assert(inputsJson["ActionMappings"].is_array());

    std::for_each(inputsJson["ActionMappings"].begin(),
                  inputsJson["ActionMappings"].end(), [this](const Json &json) {
                    InputActionKeyMappingInitializer initializer;
                    Serializer::read(json, initializer, this);
                    InputActionKeyMapping mapping{initializer};
                    mapping.postLoad({});
                    InputSettings::instance().addActionKeyMapping(mapping);
                  });
  }
}

} // namespace GEngine