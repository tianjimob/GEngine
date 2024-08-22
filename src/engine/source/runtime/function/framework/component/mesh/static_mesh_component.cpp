#include "static_mesh_component.h"
#include "function/framework/component/mesh/static_mesh_manager.h"
#include "function/framework/component/mesh/static_mesh_scene_proxy.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include "function/framework/world/world.h"
#include <memory>

namespace GEngine {

std::shared_ptr<PrimitiveSceneProxy> StaticMeshComponent::createSceneProxy() {
  return std::make_shared<StaticMeshSceneProxy>(*this);
}

void StaticMeshComponent::postLoad(std::weak_ptr<GObject> parentObject) {
  m_staticMesh = StaticMeshManager::loadStaticMesh(m_resourceName);
}

void StaticMeshComponent::tick(float deltaTime) {
  if (!m_isRegitstered) {
    auto &scene = World::GetWorld().getScene();
    scene->addPrimitive(*this);
  }
}

}