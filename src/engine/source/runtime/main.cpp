
#include "function/framework/engine/game_engine/game_engine.h"

#include <chrono>
#include <memory>

int main(int argc, char* argv[]) {
  std::shared_ptr<GEngine::GameEngine> gameEngine =
      std::make_shared<GEngine::GameEngine>();

  std::string configPath;

  gameEngine->preInit(configPath);

  gameEngine->init();

  while (!gameEngine->shouldClose()) {
    auto current_time = std::chrono::high_resolution_clock::now();
    static auto previous_time = current_time;
    float elapsed =
        std::chrono::duration<float>(current_time - previous_time).count();
    previous_time = current_time;
    elapsed = std::min(0.1f, elapsed);
    gameEngine->tick(elapsed);
  }

  gameEngine->exit();

  return 0;
}