#pragma once

#include <memory>
namespace GEngine {

class Scene;
class SceneViewFamily;

class SceneRenderer {
public:
  static std::shared_ptr<SceneRenderer> createSceneRenderer(SceneViewFamily& viewFamily);

  SceneRenderer(SceneViewFamily& viewFamily);
  virtual void render() = 0;

public:
  std::shared_ptr<Scene> scene;
  SceneViewFamily& viewFamily;
  
};

}