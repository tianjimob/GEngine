#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\actor\actor.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterActor {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::Actor"};
    classBuilder

        .addField("m_children", "std::vector<std::shared_ptr<Actor>>", & ::GEngine::Actor::m_children, "std::shared_ptr<Actor>",
                  [](const void * instance) -> size_t { return  static_cast<const ::GEngine::Actor*>(instance)->m_children.size(); },
                  [](void *instance, void *value, size_t index) {
                    static_cast< ::GEngine::Actor *>(instance) ->m_children[index] =
                        *static_cast<std::shared_ptr<Actor> *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast< ::GEngine::Actor *>(instance)->m_children[index]);
                  }, false)


        .addField("m_rootComponent", "std::shared_ptr<TransformComponent>", & ::GEngine::Actor::m_rootComponent, false)


        .addField("m_parentComponent", "std::weak_ptr<ChildActorComponent>", & ::GEngine::Actor::m_parentComponent, false)


        .addMethod("getTransform", & ::GEngine::Actor::getTransform)


        .addMethod("actorToWorld", & ::GEngine::Actor::actorToWorld)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
