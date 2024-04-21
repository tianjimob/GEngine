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

  void setEngine(std::weak_ptr<Engine> engine);

  World *getWorld() { return m_world.get(); }
  
  std::shared_ptr<LocalPlayer>& createLocalPlayer(bool shouldSpawnPlayActor);
  std::shared_ptr<GameMode> createGameMode();

  auto& getLocalPlayers() { return m_localPlayers; }
private:
  std::shared_ptr<World> m_world;
  std::vector<std::shared_ptr<LocalPlayer>> m_localPlayers;

  // pointer to engine gameinstance belongs to, initlialized after called setEngine()
  std::weak_ptr<Engine> m_engine;

};

} // namespace GEngine