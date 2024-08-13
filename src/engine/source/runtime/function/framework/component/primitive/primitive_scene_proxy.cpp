#include "primitive_scene_proxy.h"

#include "function/framework/component/primitive/primitive_component.h"

namespace GEngine {

PrimitiveSceneProxy::PrimitiveSceneProxy(PrimitiveComponent &primitive)
    : m_primitiveComponentId(primitive.getId()) {}
}