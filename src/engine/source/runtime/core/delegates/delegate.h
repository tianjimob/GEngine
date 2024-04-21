#pragma once

#include <type_traits>

#include "delegate_instance.h"


#define DECLARE_DELEGATE(delegateTypeName, Return, ...) \
  typedef Delegate<Return, __VA_ARGS__> delegateTypeName;

template <typename Return, typename... Args>
class Delegate : public DelegateInstance {
public:
  template <typename Class, typename... Vars>
  using MethodPtr =
      typename MemberFuncPtr<false, Class, Return(Args...), Vars...>::Type;
  
  using DelegateInstanceInterface = ExectuableDelegateInterface<void, Args...>;

  template <typename... Vars>
  inline static Delegate<Return, Args...> createStaticOrRaw(
      typename StaticDelegateInstance<Return(Args...),
                                      std::decay_t<Vars>...>::FuncPtr funcPtr,
      Vars &&...vars) {
    Delegate<Return, Args...> delegate;
    new (delegate)
        StaticDelegateInstance<Return(Args...), std::decay_t<Vars>...>(
            funcPtr, std::forward<Vars>(vars)...);
    return delegate;
  }

  template <typename Functor, typename... Vars>
  inline static Delegate<Return, Args...> createLambda(Functor &&functor,
                                                Vars &&...vars) {
    Delegate<Return, Args...> delegate;
    new (delegate) FunctorDelegateInstance<Return(Args...),
                                        std::remove_reference_t<Functor>,
                                        std::decay_t<Vars>...>(
        std::forward<Functor>(functor), std::forward<Vars>(vars)...);
    return delegate;
  }

  template <typename Class, typename... Vars>
  inline static Delegate<Return, Args...>
  createMemberFunction(Class *object,
                       typename MemberFunctionDelegateInstance<
                           false, Class, Return(Args...),
                           std::decay_t<Vars>...>::MemberFuncPtr memberFuncPtr,
                       Vars &&...vars) {
    Delegate<Return, Args...> delegate;
    new (delegate) MemberFunctionDelegateInstance<false, Class, Return(Args...),
                                               std::decay_t<Vars>...>(
        object, memberFuncPtr, std::forward<Vars>(vars)...);
    return delegate;
  }

  template <typename Class, typename... Vars>
  inline static Delegate<Return, Args...>
  createMemberFunction(const Class *object,
                       typename MemberFunctionDelegateInstance<
                           true, Class, Return(Args...),
                           std::decay_t<Vars>...>::MemberFuncPtr memberFuncPtr,
                       Vars &&...vars) {
    Delegate<Return, Args...> delegate;
    new (delegate) MemberFunctionDelegateInstance<true, Class, Return(Args...),
                                               std::decay_t<Vars>...>(
        object, memberFuncPtr, std::forward<Vars>(vars)...);
    return delegate;
  }
  
  template <typename... Vars>
  inline void bindStaticOrRaw(
      typename StaticDelegateInstance<Return(Args...),
                                      std::decay_t<Vars>...>::FuncPtr funcPtr,
      Vars &&...vars) {
    new (*this) StaticDelegateInstance<Return(Args...), std::decay_t<Vars>...>(
        funcPtr, std::forward<Vars>(vars)...);
  }

  template <typename Functor, typename... Vars>
  inline void bindLambda(Functor &&functor, Vars &&...vars) {
    new (*this) FunctorDelegateInstance<Return(Args...),
                                        std::remove_reference_t<Functor>,
                                        std::decay_t<Vars>...>(
        std::forward<Functor>(functor), std::forward<Vars>(vars)...);
  }

  Delegate() = default;

  ~Delegate() {  }

  Delegate(Delegate &&other) = default;

  Delegate &operator=(Delegate &&other) = default;

  Delegate(Delegate &other) { *this = other; };

  Delegate &operator=(Delegate &other) {
    if (this != &other) {
      DelegateInstanceInterface *interface = other.getDelegateInterface();
      if (interface != nullptr) {
        interface->createCopy(*this);
      }
      else unbind();
    }
    return *this;
  }

  template <typename Class, typename... Vars>
  inline void bindMemberFunction(
      Class *object,
      typename MemberFunctionDelegateInstance<
          false, Class, Return(Args...), std::decay_t<Vars>...>::MemberFuncPtr
          memberFuncPtr,
      Vars &&...vars) {
    new (*this) MemberFunctionDelegateInstance<false, Class, Return(Args...),
                                               std::decay_t<Vars>...>(
        object, memberFuncPtr, std::forward<Vars>(vars)...);
  }

  template <typename Class, typename... Vars>
  inline void bindMemberFunction(
      const Class *object,
      typename MemberFunctionDelegateInstance<
          true, Class, Return(Args...), std::decay_t<Vars>...>::MemberFuncPtr
          memberFuncPtr,
      Vars &&...vars) {
    new (*this) MemberFunctionDelegateInstance<true, Class, Return(Args...),
                                               std::decay_t<Vars>...>(
        object, memberFuncPtr, std::forward<Vars>(vars)...);
  }

  Return execute(Args... args) const {
    ExectuableDelegateInterface<Return, Args...> *interface =
        (ExectuableDelegateInterface<Return, Args...> *)getDelegateInterface();

    return interface->execute(args...);
  }

  bool safeExecute(Args... args) const {
    ExectuableDelegateInterface<Return, Args...> *interface =
        (ExectuableDelegateInterface<Return, Args...> *)getDelegateInterface();

    if (interface) {
      (void)interface->safeExecute(args...);
      return true;
    }
    return false;
  }
};
