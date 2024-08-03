#pragma once

#include <memory>
namespace GEngine {

class PrimitiveSceneInfo;

class PrimitiveSceneProxy {
public:
  void setSceneInfo(std::shared_ptr<PrimitiveSceneInfo> &sceneInfo) {
    m_sceneInfo = sceneInfo;
  }
private:
  std::shared_ptr<PrimitiveSceneInfo> m_sceneInfo;
};

}