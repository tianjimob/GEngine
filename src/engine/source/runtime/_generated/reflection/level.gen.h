#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\level\level.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterLevel {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Level"};
    classBuilder

        .setSuperClassName("GEngine::GObject")


        .addField("m_actors", "std::vector<std::shared_ptr<Actor>>", & ::GEngine::Level::m_actors, "std::shared_ptr<Actor>",
                  [](const void * instance) -> size_t { return  static_cast<const ::GEngine::Level*>(instance)->m_actors.size(); },
                  [](void *instance, void *value, size_t index) {
                    static_cast< ::GEngine::Level *>(instance) ->m_actors[index] =
                        *static_cast<std::shared_ptr<Actor> *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast< ::GEngine::Level *>(instance)->m_actors[index]);
                  }, false)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
