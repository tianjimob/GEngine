#pragma once

#include "renderer_module.h"
#include "function/framework/render/renderer/scene.h"
#include "function/framework/render/renderer/scene_renderer.h"
#include "function/framework/world/world.h"
#include <memory>

namespace GEngine {

void RendererModule::renderViewFamily(SceneViewFamily &viewFamily) {
  std::shared_ptr<World> world;

  auto &scene = viewFamily.getScene();
  if (scene) {
    world = scene->world.lock();
    if (world) {
      world->doDeferredUpdates();
    }
    scene->incrementFrameNumber();
    viewFamily.frameNumber = scene->getFrameNumber();
  }

  std::shared_ptr<SceneRenderer> sceneRenderer =
      SceneRenderer::createSceneRenderer(viewFamily);

  // todo: use render thread
  {
    sceneRenderer->render();
  }
}

std::shared_ptr<Scene>
RendererModule::allocateScene(std::shared_ptr<World> world) {
  std::shared_ptr<Scene> scene = std::make_shared<Scene>(world);
  m_allocatedScene.emplace(scene);
  return scene;
}

}