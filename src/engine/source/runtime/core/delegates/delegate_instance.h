#pragma once

#include <atomic>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <tuple>
#include <type_traits>
#include <utility>

#include "delegate_interface.h"

class DelegateInstance {
    friend class MulticastDelegateInstance;

public:
    ~DelegateInstance() { unbind(); }
    explicit DelegateInstance() : m_mem(nullptr), m_memSize(0) {}

    DelegateInstance(DelegateInstance&& other) {
        m_mem = other.m_mem;
        other.m_mem = nullptr;
        m_memSize = other.m_memSize;
        other.m_memSize = 0;
    }

    void unbind() {
        DelegateInterface* interface =
            m_memSize ? (DelegateInterface*)m_mem : nullptr;
        if (interface) {
            interface->~DelegateInterface();
            free(m_mem);
            m_mem = nullptr;
            m_memSize = 0;
        }
    }

    DelegateInterface* getDelegateInterface() const {
        return m_memSize ? (DelegateInterface*)m_mem : nullptr;
    }

private:
    friend void* operator new(size_t size, DelegateInstance& instance);
    void* m_mem;
    int m_memSize;
};

inline void* operator new(size_t size, DelegateInstance& instance) {
    DelegateInterface* interface =
        instance.m_memSize ? (DelegateInterface*)instance.m_mem : nullptr;
    if (interface) {
        interface->~DelegateInterface();
    }
    if (instance.m_memSize != size) {
        instance.m_mem = realloc(instance.m_mem, size);
        assert(instance.m_mem != nullptr);
        instance.m_memSize = size;
    }
    return instance.m_mem;
}

template <typename Return, typename... Args>
class ExectuableDelegateInterface : public DelegateInterface {
public:
    virtual void createCopy(DelegateInstance& instance) const = 0;
    virtual Return execute(Args...) const = 0;
    virtual bool safeExecute(Args...) const = 0;
};

template <typename Func, typename... Vars>
class CommonDelegateInstance;

template <typename Return, typename... Args, typename... Vars>
class CommonDelegateInstance<Return(Args...), Vars...>
    : public ExectuableDelegateInterface<Return, Args...> {
public:
    template <typename... InVars>
    explicit CommonDelegateInstance(InVars &&...vars)
        : m_payload(std::forward_as_tuple(vars...)), m_id(generateNewID()) {}

    virtual uint64_t getID() const final { return m_id; }

protected:
    std::tuple<Vars...> m_payload;
    uint64_t m_id;  // 0 denotes null delegate

private:
    uint64_t generateNewID() {
        static std::atomic_uint64_t nextID{ 0 };
        uint64_t result = ++nextID;
        if (result == 0) {
            result = ++nextID;
        }
        return result;
    }
};

template <typename Func, typename... Vars>
class StaticDelegateInstance;

template <typename Return, typename... Args, typename... Vars>
class StaticDelegateInstance<Return(Args...), Vars...>
    : public CommonDelegateInstance<Return(Args...), Vars...> {
public:
    using FuncPtr = Return(*)(Args..., Vars...);

    template <typename... InVars>
    StaticDelegateInstance(FuncPtr funcPtr, InVars... inVars)
        : CommonDelegateInstance<Return(Args...), Vars...>(
            std::forward<InVars>(inVars)...),
        m_staticFuncPtr(funcPtr) {
        assert(funcPtr != nullptr);
    }

    virtual void createCopy(DelegateInstance& instance) const {
        new (instance) StaticDelegateInstance(*this);
    }

    virtual Return execute(Args... args) const final {
        assert(m_staticFuncPtr != nullptr);
        return std::invoke(m_staticFuncPtr, args...,
            std::get<Vars>(this->m_payload)...);
    }

    virtual bool safeExecute(Args... args) const final {
        assert(m_staticFuncPtr != nullptr);
        (void)std::invoke(m_staticFuncPtr, args...,
            std::get<Vars>(this->m_payload)...);
        return true;
    }

private:
    FuncPtr m_staticFuncPtr;
};

template <typename Func, typename Functor, typename... Vars>
class FunctorDelegateInstance;

template <typename Return, typename... Args, typename Functor, typename... Vars>
class FunctorDelegateInstance<Return(Args...), Functor, Vars...>
    : public CommonDelegateInstance<Return(Args...), Vars...> {
    static_assert(std::is_same_v<Functor, std::remove_reference_t<Functor>>,
        "Functor can not be a reference!");

public:
    template <typename InFunctor, typename... InVars>
    FunctorDelegateInstance(InFunctor&& inFunctor, InVars &&...inVars)
        : CommonDelegateInstance<Return(Args...), Vars...>(
            std::forward<InVars>(inVars)...),
        m_functor(std::forward<InFunctor>(inFunctor)) {}

    virtual void createCopy(DelegateInstance& instance) const {
        new (instance) FunctorDelegateInstance(*this);
    }

    virtual Return execute(Args... args) const final {
        return std::invoke(m_functor, args..., std::get<Vars>(this->m_payload)...);
    }

    virtual bool safeExecute(Args... args) const final {
        (void)std::invoke(m_functor, args..., std::get<Vars>(this->m_payload)...);
        return true;
    }

private:
    mutable std::remove_reference_t<Functor> m_functor;
};

template <bool Const, typename Class, typename Func, typename... Vars>
class MemberFunctionDelegateInstance;

template <bool Const, typename Class, typename Return, typename... Args,
    typename... Vars>
class MemberFunctionDelegateInstance<Const, Class, Return(Args...), Vars...>
    : public CommonDelegateInstance<Return(Args...), Vars...> {
public:
    using MemberFuncPtr =
        typename MemberFuncPtr<Const, Class, Return(Args...), Vars...>::Type;

    template <typename... InVars>
    MemberFunctionDelegateInstance(Class* object, MemberFuncPtr memFuncPtr,
        InVars &&...inVars)
        : CommonDelegateInstance<Return(Args...), Vars...>(
            std::forward<InVars>(inVars)...),
        m_object(object),
        m_memFunc(memFuncPtr) {
        assert(m_memFunc != nullptr);
    }

    virtual void createCopy(DelegateInstance& instance) const {
        new (instance) MemberFunctionDelegateInstance(*this);
    }

    virtual Return execute(Args... args) const final {
        assert(m_memFunc != nullptr);
        return std::invoke(m_memFunc, (Class*)m_object, args...,
            std::get<Vars>(this->m_payload)...);
    }

    virtual bool safeExecute(Args... args) const final {
        assert(m_memFunc != nullptr);
        if (m_memFunc) {
            (void)std::invoke(m_memFunc, (Class*)m_object, args...,
                std::get<Vars>(this->m_payload)...);
            return true;
        }
        return false;
    }

private:
    void* m_object;
    MemberFuncPtr m_memFunc;
};
