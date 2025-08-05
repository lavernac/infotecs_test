#pragma once

#include <algorithm>  // для std::sort и std::reverse
#include <mutex>
#include <vector>

namespace infotecsTest {
template <typename T>
class Buffer {
 public:
  Buffer() : data(), d_copy() {}
  Buffer(int size) : data(size), d_copy(size) {}
  Buffer(int size, T value) : data(size, value), d_copy(size, value) {}
  void insert(T value);
  void insert(int index, T value);
  void remove(int index);
  void remove();
  void sort(int isReverse);
  void reverse();
  int countEven();
  int countOdd();
  void read();

 private:
  std::vector<T> data;
  std::vector<T> d_copy;
  std::mutex mtxCopy;
};
}  // namespace infotecsTest

#include "../buffer_lib/buffer_change.hpp"
#include "../buffer_lib/buffer_read.hpp"