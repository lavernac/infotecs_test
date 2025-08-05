#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "../buffer_include/task_manager.hpp"

namespace infotecsTest {

Logger::Logger(const std::string &filename) {
  namespace fs = std::filesystem;
  fs::path logdir_path = "log/";
  try {
    fs::create_directory(logdir_path);
  } catch (const fs::filesystem_error &e) {
    std::cerr << "Ошибка при создании директории " << logdir_path << ": "
              << e.what() << std::endl;
  }
  this->logfile.open(std::string(logdir_path) + filename, std::ios::app);
}

void Logger::createLog(Level level, const std::string &message) {
  std::string new_log;
  switch (level) {
    case INFO:
      new_log = "[INFO] ";
      break;
    case WARNING:
      new_log = "[WARNING] ";
      break;
    case ERROR:
      new_log = "[ERROR] ";
      break;
    default:
      new_log = "[UNKNOWN TYPE] ";
  }
  new_log += get_current_time_with_ms() + ' ' + message + "\n";
  this->logfile << new_log;
}

Logger::~Logger() {
  if (this->logfile.is_open()) this->logfile.close();
}

std::string Logger::get_current_time_with_ms() {
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_time_t);

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0')
      << std::setw(3) << ms.count();

  return oss.str();
}
}  // namespace infotecsTest