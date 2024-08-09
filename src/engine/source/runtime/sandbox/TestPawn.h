#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/pawn/pawn.h"
#include "function/framework/component/camera/camera_component.h"

#include <memory>

namespace GEngine {

CLASS(TestPawn) : public Pawn {
  REFLECTION_BODY(TestPawn)
  
public:
private:
  META_FIELD()
  std::shared_ptr<CameraComponent> m_cameraComponent;
};

}