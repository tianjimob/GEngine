#pragma once

#include <memory>

#include "core/reflection/reflection.h"
#include "function/framework/engine/scene_viewport.h"
#include "function/framework/object/object.h"
#include "function/framework/ui/input.h"


namespace GEngine {

CLASS(GameViewportClient) : public GObject {
  REFLECTION_BODY(GameViewportClient);

 public:
  void createGameViewport();
  bool inputKey(InputEvent event, ModifierKey mod, VirtualCode key);

  void setViewport(std::weak_ptr<SceneViewport> viewport) {
    m_viewport = viewport;
  }

  void draw(SceneViewport* viewport);

 private:
  // pointer to SceneViewport created by GameInstance
  std::weak_ptr<SceneViewport> m_viewport;
};

}  // namespace GEngine