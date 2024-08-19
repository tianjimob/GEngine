#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
namespace GEngine {

STRUCT(Material) : public GObject  {
  REFLECTION_BODY(Material)

  META_FIELD()
  bool isWireframe;

  bool isWireFrame() const {
    return isWireframe;
  }
};

}