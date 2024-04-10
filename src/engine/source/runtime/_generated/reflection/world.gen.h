#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\world\world.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterWorldInitializer {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::WorldInitializer"};
    classBuilder

        .setSuperClassName("GEngine::GObject")


        .addField("name", "std::string", & ::GEngine::WorldInitializer::name, false)


        .addField("levelUrls", "std::vector<std::string>", & ::GEngine::WorldInitializer::levelUrls, "std::string",
                  [](const void * instance) -> size_t { return  static_cast<const ::GEngine::WorldInitializer*>(instance)->levelUrls.size(); },
                  [](void *instance, void *value, size_t index) {
                    static_cast< ::GEngine::WorldInitializer *>(instance) ->levelUrls[index] =
                        *static_cast<std::string *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast< ::GEngine::WorldInitializer *>(instance)->levelUrls[index]);
                  }, false)


        .addField("defautLevelUrl", "std::string", & ::GEngine::WorldInitializer::defautLevelUrl, false)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterWorld {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::World"};
    classBuilder

        .setSuperClassName("GEngine::GObject")


        .addField("m_levels", "std::vector<std::shared_ptr<Level>>", & ::GEngine::World::m_levels, "std::shared_ptr<Level>",
                  [](const void * instance) -> size_t { return  static_cast<const ::GEngine::World*>(instance)->m_levels.size(); },
                  [](void *instance, void *value, size_t index) {
                    static_cast< ::GEngine::World *>(instance) ->m_levels[index] =
                        *static_cast<std::shared_ptr<Level> *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast< ::GEngine::World *>(instance)->m_levels[index]);
                  }, false)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
