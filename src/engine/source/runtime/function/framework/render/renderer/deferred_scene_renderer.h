#pragma once

#include "function/framework/render/renderer/scene_renderer.h"
namespace GEngine {

class DeferredSceneRenderer : public SceneRenderer {
public:
  void render() override;

private:
};

}