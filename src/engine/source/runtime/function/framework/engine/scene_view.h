#pragma once

#include <memory>
#include <vector>
namespace GEngine {

struct SceneViewInitOptions {};

class SceneView {
public:
  SceneView(const SceneViewInitOptions &initOptions) {}
  
private:
  
};

// A set of views into a scene which only have different view transforms and owner actors.
class SceneViewFamily {
public:
  void addSceneView(std::shared_ptr<SceneView> sceneView) {
    m_sceneViews.emplace_back(sceneView);
  }
private:
  std::vector<std::shared_ptr<SceneView>> m_sceneViews;
};

}