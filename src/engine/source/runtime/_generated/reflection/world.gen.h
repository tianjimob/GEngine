#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\world\world.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterWorld {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::World"};
    classBuilder

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


        .addField("m_persistentLevel", "std::shared_ptr<Level>", & ::GEngine::World::m_persistentLevel, false)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
