#pragma once

#include "function/framework/engine/scene_view.h"
#include <memory>
#include <set>
namespace GEngine {

class Scene;
class SceneViewFamily;
class World;

class RendererModule {
public:
  static RendererModule &instance() {
    static RendererModule instance;
    return instance;
  }

  void renderViewFamily(SceneViewFamily &viewFamily);
  std::shared_ptr<Scene> allocateScene(std::shared_ptr<World> world);

private:
  std::set<std::shared_ptr<Scene>> m_allocatedScene;
};

}