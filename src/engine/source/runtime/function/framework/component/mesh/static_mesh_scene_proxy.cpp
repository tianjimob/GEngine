#include "static_mesh_scene_proxy.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"

namespace GEngine {

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent &component)
    : PrimitiveSceneProxy(component), m_staticMesh(component.getStaticMesh()) {}
}