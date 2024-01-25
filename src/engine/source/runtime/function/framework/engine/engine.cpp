#include "engine.h"

#include "core/reflection/reflection.h"
#include "core/reflection/reflection_example.h" // for test reflection
#include <cassert>                              // for test reflection
#include <ios>                                  // for test reflection
#include <iostream>                             // for test reflection
#include "core/chrono/time_guard.h"             // for test reflection

namespace GEngine {

Engine* GlobalEngine = nullptr;

DECLARE_LOG_CATEGORY(LogEngine);

template <typename Src, typename Dst> bool dynamicTest(Src *src) {
  Dst *dst = dynamic_cast<Dst *>(src);
  if (dst)
    return true;
  else
   return false;
}

void Engine::preInit(const std::string &configPath) {
  LOG_INFO(LogEngine, "PreInitializeing Engine...");
  struct ScopeEnginePreInitGuard {
    ~ScopeEnginePreInitGuard() {
      LOG_INFO(LogEngine, "Engine PreInitialized.");
    };
  } scopeGuard;

  // register classes into Registry::instance()
  Reflection::autoRegisterAll();
  // now ClassDescriptor::m_superClass is nullptr that we can get super class
  // name (ClassDescriptor::getSuperClassName()) and set m_superClass as
  // ClassDescriptor with super class name
  Reflection::autoSetupSuperClassInfo();
  // everything about reflection infos was setup

  // for test reflection
  ReflectionExampleA exampleA;
  ReflectionExampleB exampleB;
  ReflectionExampleU exampleU;
  std::boolalpha(std::cout);
  {
    TimeGuard guard{LogEngine};
    for (int i = 0; i < 100000; ++i) {
      exampleA.isA<ReflectionExample>();   // true
      exampleB.isA<ReflectionExample>();   // true
      exampleU.isA<ReflectionExample>();   // true
      exampleA.isA<ReflectionExampleA>();  // true
      exampleB.isA<ReflectionExampleA>();  // true
      exampleU.isA<ReflectionExampleA>();  // false
      exampleA.isA<ReflectionExampleB>();  // false
      exampleB.isA<ReflectionExampleB>();  // true
      exampleU.isA<ReflectionExampleB>();  // false
      exampleA.isA<ReflectionExampleU>();  // false
      exampleB.isA<ReflectionExampleU>();  // false
      exampleU.isA<ReflectionExampleU>();  // true
      
    }
  }
  std::cout << "--------------------------------------------------------\n";
  {
    TimeGuard guard{LogEngine};
    for (int i = 0; i < 100000; ++i) {
      dynamicTest<ReflectionExampleA, ReflectionExample>(&exampleA);
      dynamicTest<ReflectionExampleB, ReflectionExample>(&exampleB);
      dynamicTest<ReflectionExampleU, ReflectionExample>(&exampleU);
      dynamicTest<ReflectionExampleA, ReflectionExampleA>(&exampleA);
      dynamicTest<ReflectionExampleB, ReflectionExampleA>(&exampleB);
      dynamicTest<ReflectionExampleU, ReflectionExampleA>(&exampleU);
      dynamicTest<ReflectionExampleA, ReflectionExampleB>(&exampleA);
      dynamicTest<ReflectionExampleB, ReflectionExampleB>(&exampleB);
      dynamicTest<ReflectionExampleU, ReflectionExampleB>(&exampleU);
      dynamicTest<ReflectionExampleA, ReflectionExampleU>(&exampleA);
      dynamicTest<ReflectionExampleB, ReflectionExampleU>(&exampleB);
      dynamicTest<ReflectionExampleU, ReflectionExampleU>(&exampleU);
    }
    
  }

  assert(0 && "for test reflection");
  exit();
  // for test
}

void Engine::init() {}

void Engine::exit() {}

} // namespace GEngine