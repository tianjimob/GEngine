#pragma once

#include <utility>

#include "delegate.h"
#include "multicast_delegate_instance.h"

#define DECLARE_MULTICAST_DELEGATE(delegateName, ...) \
  typedef MulticastDelegate<__VA_ARGS__> delegateName;

template <typename... Args>
class MulticastDelegate : public MulticastDelegateInstance {
 public:
  using DelegateInstanceInterface = ExectuableDelegateInterface<void, Args...>;

  template <typename... Vars>
  inline uint64_t addStaticOrRaw(
      typename StaticDelegateInstance<void(Args...),
                                      std::decay_t<Vars>...>::FuncPtr funcPtr,
      Vars &&...vars) {
    return addDelegate(std::move(Delegate<void, Args...>::createStaticOrRaw(
        funcPtr, std::forward<Vars>(vars)...)));
  }

  template <typename Functor, typename... Vars>
  inline uint64_t addLambda(Functor &&functor, Vars &&...vars) {
    return addDelegate(std::move(Delegate<void, Args...>::createLambda(
        std::forward<Functor>(functor), std::forward<Vars>(vars)...)));
  }

  template <typename Class, typename... Vars>
  inline uint64_t addMemberFunction(
      Class *object,
      typename MemberFunctionDelegateInstance<
          false, Class, void(Args...), std::decay_t<Vars>...>::MemberFuncPtr
          memberFuncPtr,
      Vars &&...vars) {
    return addDelegate(std::move(Delegate<void, Args...>::createMemberFunction(
        object, memberFuncPtr, std::forward<Vars>(vars)...)));
  }

  template <typename Class, typename... Vars>
  inline uint64_t addMemberFunction(
      const Class *object,
      typename MemberFunctionDelegateInstance<
          true, Class, void(Args...), std::decay_t<Vars>...>::MemberFuncPtr
          memberFuncPtr,
      Vars &&...vars) {
    return addDelegate(std::move(Delegate<void, Args...>::createMemberFunction(
        object, memberFuncPtr, std::forward<Vars>(vars)...)));
  }

  bool remove(uint64_t id) {
    if (id != 0)
      return removeDelegate(id);
    return false;
  }

  void broadcast(Args... args) {
    MulticastDelegateInstance::broadcast<DelegateInstanceInterface, Args...>(
        args...);
  }

};