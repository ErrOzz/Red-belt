#pragma once

#include <cstdlib>
#include <algorithm>

template <typename T>
class SimpleVector {
public:
  SimpleVector() : data(nullptr), capacity_(0), size_(0) {}
  explicit SimpleVector(size_t size) : data(new T[size]), capacity_(size), size_(size) {}
  ~SimpleVector() {
    delete[] data;
  }

  T& operator[](size_t index) {
    return data[index];
  }

  T* begin() {
    return data;
  }
  T* end() {
    return data + size_;
  }

  size_t Size() const {
    return size_;
  }
  size_t Capacity() const {
    return capacity_;
  }
  void PushBack(const T& value) {
    if (size_ == capacity_) {
      capacity_ = capacity_ == 0 ? 1 : capacity_ * 2;
      T* new_data = new T[capacity_];
      T* new_data_it = new_data;
      T* first = data;
      T* last = data + size_;
      while (first != last) {
        *(new_data_it++) = *(first++);
      }
      delete[] data;
      data = new_data;
    }
    data[size_++] = value;
  }

private:
  T* data;
  size_t capacity_;
  size_t size_;
};
