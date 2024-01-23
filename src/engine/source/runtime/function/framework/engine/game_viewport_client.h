#pragma once

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/ui/input.h"

namespace GEngine {

CLASS(GameViewportClient) : public GObject {
public:
  bool inputKey(InputEvent event, ModifierKey mod, VirtualCode key);
};

}