#pragma once

#include "function/framework/component/mesh/instanced_static_mesh_scene_proxy.h"
#include "function/framework/component/mesh/static_mesh_scene_proxy.h"
namespace GEngine {

InstancedStaticMeshSceneProxy::InstancedStaticMeshSceneProxy(
    InstancedStaticMeshComponent &component)
    : StaticMeshSceneProxy(component),
        m_perInstanceTransforms(component.getPerInstanceTransforms())  {}
}