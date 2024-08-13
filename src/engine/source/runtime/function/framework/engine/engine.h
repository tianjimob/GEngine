#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/player/local_player/local_player.h"
#include "function/framework/world/world.h"
#include "core/log/logger.h"
#include <memory>
#include <string>
#include <vector>

namespace GEngine {

class GameViewportClient;

CLASS(Engine) : public GObject {

  REFLECTION_BODY(Engine)

public:
  virtual ~Engine() = default;

  virtual void preInit(const std::string &configPath);
  virtual void init();
  virtual void tick(float deltaTime) = 0;
  virtual void exit();
  bool shouldClose() { return m_shouldClose; }
  std::vector<std::shared_ptr<LocalPlayer>> &getLocalPlayers();

  std::shared_ptr<World>& getWorld() { return m_world; }

protected:
  std::shared_ptr<GameViewportClient> m_gameViewportClient;
  std::shared_ptr<World> m_world;

  bool m_shouldClose{false};

private:
  void initInputSystem();
};

extern Engine* GlobalEngine;

} // namespace GEngine