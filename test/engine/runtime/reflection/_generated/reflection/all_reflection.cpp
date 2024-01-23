#include "_generated/reflection/test_reflection.gen.h"

void ::GEngine::Reflection::autoRegisterAll() {
    ::GEngine::Reflection::Register::RegisterFruit()();
    ::GEngine::Reflection::Register::RegisterFoo()();
}
