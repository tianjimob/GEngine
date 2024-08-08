#include "transform_component.h"

namespace GEngine {

const Rotator& TransformComponent::getComponentRatator() {
  if (m_cachedQuat != m_transformToWorld.rotation) {
    m_cachedQuat = m_transformToWorld.rotation;
    m_cachedRotator.pitch = m_cachedQuat.getPitch().valueDegrees();
    m_cachedRotator.yaw = m_cachedQuat.getYaw().valueDegrees();
    m_cachedRotator.roll = m_cachedQuat.getRoll().valueDegrees();
  }
  return m_cachedRotator;
}

}