#pragma once

#include "function/framework/render/renderer/scene_renderer.h"
namespace GEngine {

class SceneViewFamily;
class DeferredSceneRenderer : public SceneRenderer {
public:
  DeferredSceneRenderer(SceneViewFamily& viewFamily);
  void render() override;

private:
};

}