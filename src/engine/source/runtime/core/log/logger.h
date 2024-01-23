#pragma once
#include <memory>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <string>

namespace GEngine {

#define DECLARE_LOG_CATEGORY(categoryName)                                     \
  struct LogCategory##categoryName : public ::GEngine::LogCategory {             \
    LogCategory##categoryName() : LogCategory(#categoryName) {}                \
  } categoryName;

#ifdef DEBUG
#define LOG_HELPER(category, logLevel, ...)                                    \
  ::GEngine::Logger::instance().log(category, logLevel,                          \
                                  "[" + std::string(__FUNCTION__) + "] " +     \
                                      __VA_ARGS__)
#else
#define LOG_HELPER(category, logLevel, ...)                                    \
  ::GEngine::Logger::instance().log(category, logLevel, __VA_ARGS__)
#endif

#define LOG_TRACE(category, ...)                                               \
  LOG_HELPER(category, ::GEngine::Logger::LogLevel::trace, __VA_ARGS__);
#define LOG_DEBUG(category, ...)                                               \
  LOG_HELPER(category, ::GEngine::Logger::LogLevel::debug, __VA_ARGS__);
#define LOG_INFO(category, ...)                                                \
  LOG_HELPER(category, ::GEngine::Logger::LogLevel::info, __VA_ARGS__);
#define LOG_WARN(category, ...)                                                \
  LOG_HELPER(category, ::GEngine::Logger::LogLevel::warn, __VA_ARGS__);
#define LOG_ERROR(category, ...)                                               \
  LOG_HELPER(category, ::GEngine::Logger::LogLevel::error, __VA_ARGS__);
#define LOG_FATAL(category, ...)                                               \
  LOG_HELPER(category, ::GEngine::Logger::LogLevel::fatal, __VA_ARGS__);

class LogCategory {
public:
  LogCategory(const std::string name);
  const std::string &getName() const;

private:
  std::string m_name;
};

class Logger {
public:
  enum class LogLevel : uint8_t { trace, debug, info, warn, error, fatal };

  static Logger &instance();

public:
  Logger();

  template <typename... TARGS>
  void log(const LogCategory &category, LogLevel level,
           const std::string &format, TARGS &&...args);

private:
  std::shared_ptr<spdlog::logger> m_logger;

  template <typename... TARGS>
  void fatalCallback(const std::string &fmt, TARGS &&...args);
  
};

template <typename... TARGS>
void Logger::log(const LogCategory &category, LogLevel level,
                 const std::string &format, TARGS &&...args) {
  std::string fmt = "[" + category.getName() + "] : " + format;
  switch (level) {
  case LogLevel::trace:
    m_logger->trace(fmt, std::forward<TARGS>(args)...);
    break;
  case LogLevel::debug:
    m_logger->debug(fmt, std::forward<TARGS>(args)...);
    break;
  case LogLevel::info:
    m_logger->info(fmt, std::forward<TARGS>(args)...);
    break;
  case LogLevel::warn:
    m_logger->warn(fmt, std::forward<TARGS>(args)...);
    break;
  case LogLevel::error:
    m_logger->error(fmt, std::forward<TARGS>(args)...);
    break;
  case LogLevel::fatal:
    m_logger->critical(fmt, std::forward<TARGS>(args)...);
    fatalCallback(fmt, std::forward<TARGS>(args)...);
    break;
  default:
    break;
  }
}

template <typename... TARGS>
void Logger::fatalCallback(const std::string &fmt, TARGS &&...args) {
  const std::string format_str = fmt::format(fmt, std::forward<TARGS>(args)...);
  throw std::runtime_error(format_str);
}

} // namespace GEngine