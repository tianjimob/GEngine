#include "reflection.h"
#include <memory>
#include <vector>

ENUM(Fruit)
{Apple, Banana, Orange};

CLASS(Foo) {
  
  REFLECTION_BODY(Foo);

public:

  META_FUNCTION()
  int lerp(int value) const {
    return a * value + b;
  }

public:
  META_FIELD()
  int a = 100;
  META_FIELD()
  int b = 200;
  META_FIELD()
  Fruit fruit = Fruit::Banana;
  META_FIELD()
  float f[20] = {1.f, 2.f, 56.4f, 33.f};
  META_FIELD()
  std::vector<char> v = {65, 0, 0, 0, 68, 0, 0, 0, 69};
  META_FIELD()
  std::shared_ptr<double> d = std::make_shared<double>(201);
};