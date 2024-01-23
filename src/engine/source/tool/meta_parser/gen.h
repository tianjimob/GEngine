#pragma once

#include ""

namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterFruit {
public:
  void operator()() {

    GEngine::Reflection::EnumDescriptorBuilder enumBuilder{"Fruit"};
    enumBuilder
        .addEnum("Apple", static_cast<int64_t>(Fruit::Apple))
        .addEnum("Banana", static_cast<int64_t>(Fruit::Banana))
        .addEnum("Orange", static_cast<int64_t>(Fruit::Orange))
    ;

    REGISTER_ENUM(enumBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
namespace GEngine {
namespace Reflection {
namespace Register {
class RegisterFoo {
public:
  void operator()() {

    GEngine::Reflection::ClassDescriptorBuilder classBuilder{"Foo"};
    classBuilder

        .addField("a", "int", &Foo::a, false)


        .addField("b", "int", &Foo::b, false)


        .addField("fruit", "Fruit", &Foo::fruit, true)


        .addField("f", "float[20]", &Foo::f, "float",
                  [](const void * instance) -> size_t { return 20; },
                  [](void *instance, void *value, size_t index) {
                    static_cast<Foo *>(instance) -> f[index] =
                        *static_cast<float *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast<Foo *>(instance)->f[index]);
                  }, false)


        .addField("v", "std::vector<char>", &Foo::v, "char",
                  [](const void * instance) -> size_t { return  static_cast<const Foo*>(instance)->v.size(); },
                  [](void *instance, void *value, size_t index) {
                    static_cast<Foo *>(instance) ->v[index] =
                        *static_cast<char *>(value);
                  },
                  [](void *instance, size_t index) -> void * {
                    return static_cast<void *>(
                        &static_cast<Foo *>(instance)->v[index]);
                  }, false)


        .addMethod("lerp(int, const std::string &)", &Foo::lerp(int, const std::string &))

    ;

    REGISTER_CLASS(classBuilder);

  }
};
} // namespace Register
} // namespace Reflection
} // namespace GEngine
