#pragma once

#include <stdexcept>
#include <array>

using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t size = 0);

  T& operator[](size_t index);
  const T& operator[](size_t index) const;

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  size_t Size() const;
  size_t Capacity() const;

  void PushBack(const T& value);
  T PopBack();

private:
  size_t size_;
  array<T, N> data_;
};

template <typename T, size_t N>
StackVector<T, N>::StackVector(size_t size) : size_(size) {
  if (size_ > N) {
    throw invalid_argument("The size can't be larger than the capacity");
  }
}

template <typename T, size_t N>
T& StackVector<T, N>::operator[](size_t index) {
  return data_[index];
}

template <typename T, size_t N>
const T& StackVector<T, N>::operator[](size_t index) const {
  return data_[index];
}

template <typename T, size_t N>
T* StackVector<T, N>::begin() {
  return data_.begin();
}


template <typename T, size_t N>
T* StackVector<T, N>::end() {
  return data_.end();
}

template <typename T, size_t N>
const T* StackVector<T, N>::begin() const {
  return data_.begin();
}

template <typename T, size_t N>
const T* StackVector<T, N>::end() const {
  return data_.end();
}

template <typename T, size_t N>
size_t StackVector<T, N>::Size() const {
  return size_;
}

template <typename T, size_t N>
size_t StackVector<T, N>::Capacity() const {
  return N;
}

template <typename T, size_t N>
void StackVector<T, N>::PushBack(const T& value) {
  if (Size() == Capacity()) {
    throw overflow_error("The size has reached the capacity");
  }
  data_[size_++] = value;
}

template <typename T, size_t N>
T StackVector<T, N>::PopBack() {
  if (!Size()) {
    throw underflow_error("The size is zero");
  }
  return data_[--size_];
}
