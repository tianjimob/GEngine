#pragma once

#include "function/framework/component/mesh/static_mesh.h"
#include "function/framework/component/mesh/static_mesh_component.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include <memory>
namespace GEngine {

class StaticMeshSceneProxy : public PrimitiveSceneProxy {
public:
  StaticMeshSceneProxy(StaticMeshComponent &component);
  
protected:
  std::shared_ptr<StaticMesh> m_staticMesh;
};

}