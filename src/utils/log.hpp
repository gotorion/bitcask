#ifndef LOG_H_
#define LOG_H_
#include <spdlog/spdlog.h>
namespace bitcask::utils::log {
class Logger {
 public:
  Logger& GetInst();
  void Init();

 private:
};

/**
 * @description: Singleton get instance
 * @return {Logger&}
 */
inline Logger& Logger::GetInst() {
  static Logger logger;
  return logger;
}

/**
 * @description: Init Logger
 * @return {void}
 */
inline void Logger::Init() {
  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

}  // namespace bitcask::utils::log
#endif
