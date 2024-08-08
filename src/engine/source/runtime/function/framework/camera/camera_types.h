#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
namespace GEngine {

STRUCT(PostProcessSetting) : public GObject {
  REFLECTION_BODY(PostProcessSetting)
};

}