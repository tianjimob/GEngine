#include "scene_renderer.h"
#include "function/framework/engine/scene_view.h"
#include "function/framework/render/renderer/deferred_scene_renderer.h"
#include <cassert>
#include <memory>

namespace GEngine {

std::shared_ptr<SceneRenderer>
SceneRenderer::createSceneRenderer(SceneViewFamily &viewFamily) {
  // now only support DeferredSceneRenderer
  return std::make_shared<DeferredSceneRenderer>(viewFamily);
}

SceneRenderer::SceneRenderer(SceneViewFamily &viewFamily)
    : scene(viewFamily.getScene()), viewFamily(viewFamily) {
  assert(scene);
}
}