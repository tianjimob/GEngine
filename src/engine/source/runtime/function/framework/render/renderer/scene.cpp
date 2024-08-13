#include "scene.h"

#include <memory>

#include "function/framework/component/primitive/primitive_component.h"
#include "function/framework/component/primitive/primitive_scene_info.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"

namespace GEngine {

Scene::Scene(std::shared_ptr<World> world)
    :world(world),m_sceneFrameNumber(0) {}

void Scene::addPrimitive(PrimitiveComponent &primitive) {
  std::shared_ptr<PrimitiveSceneProxy> sceneProxy =
      primitive.createSceneProxy();
  primitive.setPrimitiveSceneProxy(sceneProxy);
  std::shared_ptr<PrimitiveSceneInfo> sceneInfo =
      std::make_shared<PrimitiveSceneInfo>(primitive, shared_from_this());
  sceneProxy->setSceneInfo(sceneInfo);
  addedPrimitiveInfos.emplace(sceneInfo.get());
}

void Scene::updateAllPrimitiveInfos() {}

}  // namespace GEngine