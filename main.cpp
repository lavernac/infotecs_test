#include <iostream>

#include "buffer_include/buffer.hpp"
#include "buffer_include/task_manager.hpp"

int main() {
  infotecsTest::Buffer<int> buff;
  infotecsTest::TaskManager tChange("change.log");
  infotecsTest::TaskManager tRead("read.log");
  int input = -1;
  bool stopThread = false;
  std::cout << "1 - add 200 000 elements in buffer.\n"
            << "2 - remove 200 000 elements.\n"
            << "3 - sort buffer.\n"
            << "4 - reverse data.\n"
            << "5 - count even indexes of buffer.\n"
            << "6 - count odd.\n"
            << "7 - read (count elements num).\n"
            << "8 - exit.\n\n";

  while (stopThread != true) {
    std::cout << "write num between 1 and 8\n";
    std::cin >> input;
    switch (input) {
      case 1:
        tChange.logger.createLog(infotecsTest::Logger::INFO,
                                 "Inserting started");
        for (int i = 0; i < 200000; i++)
          tChange.addTask([&buff, &i]() { buff.insert(i); });
        break;
      case 2:
        tChange.logger.createLog(infotecsTest::Logger::INFO,
                                 "Removing started");
        for (int i = 0; i < 200000; i++)
          tChange.addTask([&buff]() { buff.remove(); });
        break;
      case 3: {
        int isReverse = -1;
        std::cout << "1 - reverse sort\n"
                  << "0 - default sort\n";
        std::cin >> isReverse;
        tChange.logger.createLog(infotecsTest::Logger::INFO, "Sorting started");
        tChange.addTask([&buff, isReverse]() { buff.sort(isReverse); });
        break;
      }
      case 4:
        tChange.logger.createLog(infotecsTest::Logger::INFO,
                                 "Reversing started");
        tChange.addTask([&buff]() { buff.reverse(); });
        break;
      case 5:
        tChange.logger.createLog(infotecsTest::Logger::INFO,
                                 "Counting even started");
        tChange.addTask(
            [&buff]() { std::cout << buff.countEven() << " even elements\n"; });
        break;
      case 6:
        tChange.logger.createLog(infotecsTest::Logger::INFO,
                                 "Counting odd started");
        tChange.addTask(
            [&buff]() { std::cout << buff.countOdd() << " odd elements\n"; });
        break;
      case 7:
        tRead.logger.createLog(infotecsTest::Logger::INFO, "Reading started");
        tRead.addTask([&buff]() { buff.read(); });
        break;
      case 8:
        tChange.stop();
        tRead.stop();
        stopThread = true;
        std::cout << "good bye :)\n";
        break;
      default:
        std::cout << "Wrong input!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  tRead.logger.createLog(infotecsTest::Logger::INFO, "===== EXIT =====");
  tChange.logger.createLog(infotecsTest::Logger::INFO, "===== EXIT =====");
}