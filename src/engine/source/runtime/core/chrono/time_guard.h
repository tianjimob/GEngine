#pragma once

#include <chrono>

#include "core/log/logger.h"

#ifdef DEBUG
#define SCOPE_TIME_GUARD(logCategory)                                          \
  ::GEngine::TimeGuard scopeTimeGuard##logCategory{logCategory};
#else
#define SCOPE_TIME_GUARD(logCategory)
#endif

namespace GEngine {

class TimeGuard {
public:
  TimeGuard(const LogCategory &logCategory)
      : m_start(std::chrono::high_resolution_clock::now()),
        m_logCategory(logCategory) {}
  ~TimeGuard() {
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_start);
    LOG_INFO(m_logCategory, "elapsed {}us", elapsed.count());
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
  const LogCategory& m_logCategory;
};

}