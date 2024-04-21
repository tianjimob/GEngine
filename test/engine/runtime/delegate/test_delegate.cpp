#include "catch_amalgamated.hpp"
#include "core/delegates/delegate.h"
#include "core/delegates/multicast_delegate.h"

DECLARE_DELEGATE(Delegate1, void, int)

static int test4 = 4;
void add(int arg, int var) { test4 += arg + var; }

TEST_CASE("SingleCast Delegate", "[Delegate]") {
  Delegate1 delegate1;

  int test1 = 1;
  delegate1.bindLambda([&test1](int arg, int var) { test1 += arg + var; }, 1);
  delegate1.execute(1);
  REQUIRE(test1 == 3);

  static int test2 = 2;
  class AddNum {
  public:
    static void addNum(int arg, int var) { test2 += arg + var; }

    void add(int arg, int var) { test3 += arg + var; }
    int test3 = 3;
  };

  delegate1.bindStaticOrRaw(&AddNum::addNum, 2);
  delegate1.execute(2);
  REQUIRE(test2 == 6);

  AddNum addNum3;
  delegate1.bindMemberFunction(&addNum3, &AddNum::add, 3);
  delegate1.execute(3);
  REQUIRE(addNum3.test3 == 9);

  delegate1.bindStaticOrRaw(&add, 4);
  delegate1.execute(4);
  REQUIRE(test4 == 12);

  delegate1.unbind();
  REQUIRE(delegate1.safeExecute(4) == false);
}

DECLARE_MULTICAST_DELEGATE(Delegate2, int)

static int test5 = 4;
void add2(int arg, int var) { test5 += arg + var; }

TEST_CASE("MultiCast Delegate", "[Delegate]") {
  Delegate2 delegate2;

  int test1 = 1;
  delegate2.addLambda([&test1](int arg, int var) { test1 += arg + var; }, 1);
  REQUIRE(delegate2.size() == 1);

  static int test2 = 2;
  class AddNum {
  public:
    static void addNum(int arg, int var) { test2 += arg + var; }

    void add(int arg, int var) { test3 += arg + var; }
    int test3 = 3;
  };

  delegate2.addStaticOrRaw(&AddNum::addNum, 2);
  REQUIRE(delegate2.size() == 2);
  
  AddNum addNum3;
  delegate2.addMemberFunction(&addNum3, &AddNum::add, 3);
  REQUIRE(delegate2.size() == 3);

  auto newDelete = delegate2.addStaticOrRaw(&add2, 4);
  REQUIRE(delegate2.size() == 4);

  delegate2.broadcast(5);
  REQUIRE(test1 == 7);
  REQUIRE(test2 == 9);
  REQUIRE(addNum3.test3 == 11);
  REQUIRE(test5 == 13);

  delegate2.remove(newDelete);
  REQUIRE(delegate2.size() == 3);

  delegate2.clear();
  REQUIRE(delegate2.size() == 0);
}
