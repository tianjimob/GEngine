#pragma once

#include "core/math/transform.h"
#include "function/framework/component/mesh/instanced_static_mesh_component.h"
#include "function/framework/component/mesh/static_mesh_scene_proxy.h"
#include <vector>
namespace GEngine {

class InstancedStaticMeshSceneProxy : public StaticMeshSceneProxy {
public:
  InstancedStaticMeshSceneProxy(InstancedStaticMeshComponent &component);

protected:
  std::vector<Transform> m_perInstanceTransforms;
};

}