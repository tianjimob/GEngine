#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
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

  std::shared_ptr<World> loadWorld();

protected:
  std::shared_ptr<GameViewportClient> m_gameViewportClient;

  bool m_shouldClose { false };
};

extern Engine* GlobalEngine;

} // namespace GEngine