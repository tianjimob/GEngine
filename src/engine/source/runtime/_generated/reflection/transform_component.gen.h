#pragma once

#include "D:\code\GEngine\src\engine\source\runtime\function\framework\component\transform\transform_component.h"

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterTransformComponent {
public:
  void operator()() {

    ::GEngine::Reflection::ClassDescriptorBuilder classBuilder{"GEngine::TransformComponent"};
    classBuilder

        .addField("m_attchParent", "std::weak_ptr<TransformComponent>", & ::GEngine::TransformComponent::m_attchParent, false)


        .addField("m_attachChildren", "std::vector<std::shared_ptr<TransformComponent>>", & ::GEngine::TransformComponent::m_attachChildren, "std::shared_ptr<TransformComponent>",
                  [](const void * instance) -> size_t { return  static_cast<const ::GEngine::TransformComponent*>(instance)->m_attachChildren.size(); },
                  [](void *instance, void *value, size_t index) {
                    static_cast< ::GEngine::TransformComponent *>(instance) ->m_attachChildren[index] =
                        *static_cast<std::shared_ptr<TransformComponent> *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast< ::GEngine::TransformComponent *>(instance)->m_attachChildren[index]);
                  }, false)

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
