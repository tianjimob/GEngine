#include "instanced_static_mesh_component.h"
#include "function/framework/component/mesh/instanced_static_mesh_scene_proxy.h"
#include <memory>

namespace GEngine {

std::shared_ptr<PrimitiveSceneProxy>
InstancedStaticMeshComponent::createSceneProxy() {
  return std::make_shared<InstancedStaticMeshSceneProxy>(*this);
}

}