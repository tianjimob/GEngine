#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"

namespace GEngine {

class Pawn;

CLASS(GameMode) : public Actor {

  REFLECTION_BODY(GameMode)

public:
  META_FIELD()
  Reflection::ClassDescriptor* defaultPawnClass;

private:
  
};

}