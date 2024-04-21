#include "engine.h"

#include "core/reflection/reflection.h"
#include "function/framework/input/key.h"
#include "core/misc/config_cache_ini.h"
#include "resource/resource_path.h"
#include "core/misc/json.h"
#include "core/serializer/serializer.h"
#include "function/framework/input/input_settings.h"
#include <algorithm>
#include <string>
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
  // now ClassDescriptor::m_superClass is nullptr that we can get super class
  // name (ClassDescriptor::getSuperClassName()) and set m_superClass as
  // ClassDescriptor with super class name
  Reflection::autoSetupSuperClassInfo();
  // everything about reflection infos was setup

  {
    Keys::init();

    std::vector<std::string> axisMappings;
    std::vector<std::string> actionMappings;
    ConfigCacheIni::instance().getStrings("Mapping", "AxisMappings",
                                          ResourcePath::inputIni, axisMappings);
    ConfigCacheIni::instance().getStrings("Mapping", "ActionMappings",
                                          ResourcePath::inputIni, actionMappings);
    auto getJsonFromValue = [](const std::string &value) {
      std::string jsonStr;
      jsonStr.resize(value.size() + 2);
      int index = 1;
      for (auto ch : value) {
        if (ch == '=')
          jsonStr[index++] = ':';
        else
          jsonStr[index++] = ch;
      }
      jsonStr[0] = '{';
      jsonStr.back() = '}';
      return Json::parse(jsonStr);
    };
    std::for_each(axisMappings.begin(), axisMappings.end(),
                  [&getJsonFromValue](const std::string &str) {
                    auto json = getJsonFromValue(str);
                    InputAxisKeyMapping mapping;
                    Serializer::read(json, mapping);
                    mapping.postLoad({});
                    InputSettings::instance().addAxisKeyMapping(mapping);
                  });
    std::for_each(actionMappings.begin(), actionMappings.end(),
                  [&getJsonFromValue](const std::string &str) {
                    auto json = getJsonFromValue(str);
                    InputActionKeyMapping mapping;
                    Serializer::read(json, mapping);
                    mapping.postLoad({});
                    InputSettings::instance().addActionKeyMapping(mapping);
                  });
  }
}

void Engine::init() {}

void Engine::exit() {}

} // namespace GEngine