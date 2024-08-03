#include "static_mesh_component.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include <memory>

namespace GEngine {

std::shared_ptr<PrimitiveSceneProxy> StaticMeshComponent::createSceneProxy() {
  return std::make_shared<PrimitiveSceneProxy>(*this);
}

void StaticMeshComponent::postLoad(std::weak_ptr<GObject> parentObject) {
  
}

}