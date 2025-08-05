#pragma once
#include <iostream>

#include "../buffer_include/buffer.hpp"

namespace infotecsTest {
template <typename T>
void Buffer<T>::read() {
  int count = 0;
  std::unique_lock<std::mutex> lock(mtxCopy);

  for (auto i = d_copy.begin(); i != d_copy.end(); i++) {
    count++;
  }
  std::cout << count << " elements in buffer now\n";
}
}  // namespace infotecsTest