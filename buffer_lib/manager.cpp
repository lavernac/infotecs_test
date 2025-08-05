#include "../buffer_include/task_manager.hpp"

namespace infotecsTest {
TaskManager::TaskManager(const std::string &filename) : logger(filename) {
  this->t = std::thread([this]() {
    while (true) {
      std::unique_lock<std::mutex> lock(this->mtxTask);

      tasksCV.wait(lock, [this]() { return !tasks.empty() || stopThread; });
      if (stopThread && this->tasks.empty()) {
        return;
      }

      while (!tasks.empty()) {
        auto func = std::move(tasks.front());
        tasks.pop();
        lock.unlock();
        func();
        lock.lock();
      }
      logger.createLog(infotecsTest::Logger::INFO, "Tasks completed");
    }
  });
}

void TaskManager::stop() {
  this->stopThread = true;
  tasksCV.notify_one();
}

void TaskManager::addTask(std::function<void()> func) {
  std::unique_lock<std::mutex> lock(mtxTask);
  bool wasEmpty = tasks.empty();
  tasks.push(func);
  if (wasEmpty) tasksCV.notify_one();
}

TaskManager::~TaskManager() {
  this->t.join();
  logger.createLog(infotecsTest::Logger::INFO, "===== EXIT =====");
}
}  // namespace infotecsTest