#pragma once

#include <memory>
namespace GEngine {

class Scene;

class SceneRenderer {
public:
  std::unique_ptr<Scene> m_scene;
  virtual void render() = 0;
};

}