#include "deferred_scene_renderer.h"
#include "function/framework/engine/scene_view.h"
#include "function/framework/render/renderer/scene_renderer.h"
namespace GEngine {

DeferredSceneRenderer::DeferredSceneRenderer(SceneViewFamily &viewFamily)
    : SceneRenderer(viewFamily) {}

void DeferredSceneRenderer::render() {
  scene->updateAllPrimitiveInfos();
}

}