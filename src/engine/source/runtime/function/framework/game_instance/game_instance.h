#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/world/world.h"
#include "function/framework/player/local_player/local_player.h"
#include "function/framework/actor/game_mode/game_mode.h"
#include <memory>
#include <vector>

namespace GEngine {

class Engine;

CLASS(GameInstance) : public GObject {

  REFLECTION_BODY(GameInstance)

public:
  void preInit();
  void init();
  void tick(float deltaTime);
  void exit();

  void setEngine(Engine* engine);

  std::shared_ptr<World> getWorld();
  std::shared_ptr<LocalPlayer> createLocalPlayer();
  std::shared_ptr<GameMode> createGameMode();
private:
  std::shared_ptr<World> m_world;
  std::vector<std::shared_ptr<LocalPlayer>> m_localPlayers;

  // pointer to engine gameinstance belongs to, initlialized after calling setEngine()
  Engine* m_engine;
};

} // namespace GEngine