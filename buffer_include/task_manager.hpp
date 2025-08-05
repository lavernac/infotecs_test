#pragma once
#include <condition_variable>
#include <fstream>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace infotecsTest {

class Logger {
 public:
  enum Level { ERROR, WARNING, INFO };

  explicit Logger(const std::string &filename);
  void createLog(Level level, const std::string &message);

 private:
  std::string get_current_time_with_ms();

  std::string filename;
  std::string logdir_path = "log/";
};

class TaskManager {
 public:
  explicit TaskManager(const std::string &filename);
  ~TaskManager();
  void stop();
  void addTask(std::function<void()> func);

  Logger logger;

 private:
  std::thread t;
  std::queue<std::function<void()>> tasks;
  std::mutex mtxTask;
  std::condition_variable tasksCV;
  bool stopThread = false;
};

}  // namespace infotecsTest
