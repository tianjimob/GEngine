#include "camera_component.h"

namespace GEngine {

void CameraComponent::getCameraView(float deltaTime, CameraViewInfo &viewInfo) {
  viewInfo.location = m_transformToWorld.position;
  viewInfo.rotator = getComponentRatator();
  viewInfo.fov = m_fieldOfView;
  viewInfo.aspectRatio = m_aspectRation;
  viewInfo.postProcessBlendWeight = m_postProcessBlendWeight;
  viewInfo.postProcessSetting = m_postProcessSetting;
}

}  // namespace GEngine