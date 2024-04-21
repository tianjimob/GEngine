#pragma once

#include <cstdint>

class DelegateInterface {
 public:
   virtual ~DelegateInterface() = default;
   virtual uint64_t getID() const = 0;
};

template <bool Const, typename Class, typename Func, typename... Vars>
struct MemberFuncPtr;

template <typename Class, typename Return, typename... Args, typename... Vars>
struct MemberFuncPtr<false, Class, Return(Args...), Vars...> {
  typedef Return (Class::*Type)(Args..., Vars...);
};

template <typename Class, typename Return, typename... Args, typename... Vars>
struct MemberFuncPtr<true, Class, Return(Args...), Vars...> {
  typedef Return (Class::*Type)(Args..., Vars...) const;
};
