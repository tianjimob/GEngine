#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/camera/camera_types.h"
#include "function/framework/camera/player_camera_manager.h"
#include "function/framework/component/transform/transform_component.h"
namespace GEngine {

CLASS(CameraComponent) : public TransformComponent {
public:
  virtual void getCameraView(float deltaTime, CameraViewInfo &viewInfo);

private:
  META_FIELD()
  float m_fieldOfView;

  META_FIELD()
  float m_aspectRation;

  META_FIELD()
  float m_postProcessBlendWeight;

  META_FIELD()
  PostProcessSetting m_postProcessSetting;
};

}