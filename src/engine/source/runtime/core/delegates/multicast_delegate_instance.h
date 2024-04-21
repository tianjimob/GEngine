#pragma once

#include <list>
#include <mutex>
#include <utility>
#include <cstdint>

#include "delegate_instance.h"

class MulticastDelegateInstance {
public:

  uint64_t addDelegate(DelegateInstance &&delegateInstance) {
    std::lock_guard<std::mutex> lk{m_mtx};

    uint64_t result = 0; 
    if (auto* instance = delegateInstance.getDelegateInterface(); instance) {
      result = instance->getID();
      m_invocations.emplace_back(std::move(delegateInstance));
      return result;
    }
    return result;
  }

  bool removeDelegate(uint64_t id) {
    std::lock_guard<std::mutex> lk{m_mtx};

    auto itEnd = m_invocations.end();
    for (auto it = m_invocations.begin(); it != itEnd; ++it) {
      auto &delegateInstance = *it;
      if (auto* interface = delegateInstance.getDelegateInterface(); interface->getID() == id) {
        delegateInstance.unbind();
        m_invocations.erase(it);
        return true;
      }
    }
    return false;
  }

  void clear() {
    std::lock_guard<std::mutex> lk{m_mtx};

    auto itEnd = m_invocations.end();
    for (auto it = m_invocations.begin(); it != itEnd; ++it) {
      auto &delegateInstance = *it;
      delegateInstance.unbind();
    }
    m_invocations.clear();
  }

  // invoke in reverse order
  template <typename DelegateInstanceInterface, typename... Args> void broadcast(Args ... args) const {
    auto itEnd = m_invocations.rend();
    for (auto it = m_invocations.rbegin(); it != itEnd; ++it) {
      const DelegateInstance &delegateInstance = *it;
      if (auto *interface = delegateInstance.getDelegateInterface();
          ((DelegateInstanceInterface *)interface)->safeExecute(args...))
        ;
    }
  }

  auto size() { return m_invocations.size(); }

private:
  std::list<DelegateInstance> m_invocations;

  mutable std::mutex m_mtx;
};