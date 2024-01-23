#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/engine/engine.h"
#include "function/framework/game_instance/game_instance.h"

#include <memory>

namespace GEngine {

CLASS(EditorEngine) : public Engine {

  REFLECTION_BODY(EditorEngine)

public:
private:
  std::shared_ptr<GameInstance> m_playInstance;
  std::shared_ptr<GameInstance> m_editorInstance;
  
};

}