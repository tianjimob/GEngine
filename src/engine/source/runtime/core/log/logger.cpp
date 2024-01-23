#include "logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace GEngine {
LogCategory::LogCategory(const std::string name) : m_name(name) {}

const std::string& LogCategory::getName() const
 { return m_name; }

Logger &Logger::instance() {
  static Logger logger;
  return logger;
}

Logger::Logger() {

  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto file_sink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>("mylog.txt", true);

  std::vector<spdlog::sink_ptr> sinks{stdout_sink, file_sink};
  m_logger = std::make_shared<spdlog::logger>(
      "DefaultLogger", sinks.begin(), sinks.end());
  m_logger->set_level(spdlog::level::trace);
  m_logger->set_pattern("%Y-%m-%d %H:%M:%S.%e  %l %v");
}
} // namespace GEngine