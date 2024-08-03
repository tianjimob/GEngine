#pragma once

#include <memory>
namespace GEngine {

class Scene;

class SceneRenderer {
public:
  virtual void render() = 0;

protected:
  std::unique_ptr<Scene> m_scene;
  
};

}