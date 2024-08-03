#include "scene.h"
#include "function/framework/component/primitive/primitive_component.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include <memory>

namespace GEngine {

void Scene::addPrimitive(PrimitiveComponent &primitive) {
  std::shared_ptr<PrimitiveSceneProxy> sceneProxy =
      primitive.createSceneProxy();
  primitive.setPrimitiveSceneProxy(sceneProxy);
  std::shared_ptr<PrimitiveSceneInfo> sceneInfo =
      std::make_shared<PrimitiveSceneInfo>(primitive, shared_from_this());
  sceneProxy->setSceneInfo(sceneInfo);
  addedPrimitiveInfos.emplace(sceneInfo.get());
}

void Scene::updateAllPrimitiveInfos() {
  
}

}