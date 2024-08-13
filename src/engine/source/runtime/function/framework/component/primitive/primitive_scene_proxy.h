#pragma once

#include <memory>
namespace GEngine {

class PrimitiveSceneInfo;
class PrimitiveComponent;

class PrimitiveSceneProxy {
public:
 PrimitiveSceneProxy(PrimitiveComponent& primitive);
 void setSceneInfo(std::shared_ptr<PrimitiveSceneInfo>& sceneInfo) {
   m_sceneInfo = sceneInfo;
  }

private:
  uint32_t m_primitiveComponentId;
  std::shared_ptr<PrimitiveSceneInfo> m_sceneInfo;
};

}