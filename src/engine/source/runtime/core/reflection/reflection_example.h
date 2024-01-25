#pragma once

#include "reflection.h"
#include "function/framework/object/object.h"

namespace GEngine {

CLASS(ReflectionExample) : public GObject {

  REFLECTION_BODY(ReflectionExample)

public:
  virtual ~ReflectionExample() = default;
};

CLASS(ReflectionExampleA) : public ReflectionExample {

  REFLECTION_BODY(ReflectionExampleA)

public:
  virtual ~ReflectionExampleA() = default;
};

CLASS(ReflectionExampleB) : public ReflectionExampleA {

  REFLECTION_BODY(ReflectionExampleB)

public:
  virtual ~ReflectionExampleB() = default;
};

CLASS(ReflectionExampleU) : public ReflectionExample {

  REFLECTION_BODY(ReflectionExampleU)

public:
  virtual ~ReflectionExampleU() = default;
};

}