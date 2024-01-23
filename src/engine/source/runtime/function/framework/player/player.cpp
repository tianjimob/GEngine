#include "player.h"

namespace GEngine {
void Player::switchController(
    std::shared_ptr<PlayerController> playerController) {
  if (m_playerController) {
    m_playerController->setPlayer(nullptr);
  }
  playerController->setPlayer(this);
  m_playerController = playerController;
}

std::shared_ptr<PlayerController> Player::getPlayerController() const
{
    return m_playerController;
}

} // namespace GEngine