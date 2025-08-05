#pragma once
#include <iostream>

#include "../buffer_include/buffer.hpp"
namespace infotecsTest {
// Вставка по индексу
template <typename T>
void Buffer<T>::insert(int index, T value) {
  auto iter = this->data.begin();
  iter += index;
  this->data.insert(iter, value);
  std::cout << "insert complete\n";

  std::lock_guard<std::mutex> lock(this->mtxCopy);
  this->d_copy = this->data;
}

// Вставка в конец
template <typename T>
void Buffer<T>::insert(T value) {
  this->data.push_back(value);

  std::lock_guard<std::mutex> lock(this->mtxCopy);
  this->d_copy = this->data;
}

// Удаляет элемент по индексу
template <typename T>
void Buffer<T>::remove(int index) {
  auto iter = this->data.begin();
  iter += index;
  this->data.erase(iter);

  std::lock_guard<std::mutex> lock(this->mtxCopy);
  this->d_copy = this->data;
}

// Удаляет последний элемент
template <typename T>
void Buffer<T>::remove() {
  this->data.pop_back();

  std::lock_guard<std::mutex> lock(this->mtxCopy);
  this->d_copy = this->data;
}

/*
 *isReverse = 1 - сортировка от большего к меньшему
 *
 *isReverse = 0 - сортировка от меньшего к большему
 *
 * в остальных случаях не изменяет порядок
 */
template <typename T>
void Buffer<T>::sort(int isReverse) {
  if (isReverse == 0)
    std::sort(this->data.begin(), this->data.end());
  else if (isReverse == 1)
    std::sort(this->data.rbegin(), this->data.rend());

  std::lock_guard<std::mutex> lock(this->mtxCopy);
  this->d_copy = this->data;
}

template <typename T>
void Buffer<T>::reverse() {
  std::reverse(this->data.begin(), this->data.end());

  std::lock_guard<std::mutex> lock(this->mtxCopy);
  this->d_copy = this->data;
}

/*
 * Здесь можно было бы вычислить кол-во чётных элементов по формуле
 * (this->data.size() / 2) + this->data.size() % 2
 * так было бы в разы быстрее, но в задании сказано именно посчитать, а не
 * вычислить, поэтому предлагаю такой вариант
 */
template <typename T>
int Buffer<T>::countEven() {
  int counter = 0;
  for (auto i = 0LU; i < this->data.size(); i++) {
    if ((i % 2) == 0) counter++;
  }
  return counter;
}

/*
 * Здесь можно было бы вычислить кол-во чётных элементов по формуле
 * (this->data.size() / 2)
 * ситуация такая же как с подсчётом чётных индексов
 *
 * помимо этой формулы можно воспользоваться ещё одним способом
 * можно избавиться от if в цикле путём добавления continue в конец цикла, а
 * counter инкрементировать каждый раз, так тоже получится быстрее, но обычно
 * стараются не использовать continue
 */
template <typename T>
int Buffer<T>::countOdd() {
  int counter = 0;
  for (auto i = 0LU; i < this->data.size(); i++) {
    if ((i % 2) == 1) counter++;
  }
  return counter;
}
}  // namespace infotecsTest