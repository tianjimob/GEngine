#include "test_reflection.h"
#include "catch_amalgamated.hpp"
#include <memory>

TEST_CASE("reflection for class and enum", "[reflection]") {
  using namespace GEngine::Reflection;

  autoRegisterAll();

  auto &classMeta = Registry::instance().getClass("Foo");

  Foo foo;
  auto &fields = classMeta.getFieldsList();
  auto &methods = classMeta.getMethodsList();
  EnumDescriptor &enumMeta = Registry::instance().getEnum("Fruit");
  auto &enums = enumMeta.getEnumsList();

  REQUIRE(fields.size() == 6);
  REQUIRE(methods.size() == 1);
  REQUIRE(enums.size() == 3);

  REQUIRE(fields[0].getOffsetInClass() == offsetof(Foo, a));
  REQUIRE(fields[1].getOffsetInClass() == offsetof(Foo, b));
  REQUIRE(fields[2].getOffsetInClass() == offsetof(Foo, fruit));
  REQUIRE(fields[3].getOffsetInClass() == offsetof(Foo, f));
  REQUIRE(fields[4].getOffsetInClass() == offsetof(Foo, v));
  REQUIRE(fields[5].getOffsetInClass() == offsetof(Foo, d));

  std::shared_ptr<double>& d = *static_cast<std::shared_ptr<double>*>(fields[5].get(&foo));
  REQUIRE(d.get() != nullptr);
  REQUIRE((*d = 100) == 100);
  REQUIRE(*(d = std::make_shared<double>(2000)) == 2000);
}
