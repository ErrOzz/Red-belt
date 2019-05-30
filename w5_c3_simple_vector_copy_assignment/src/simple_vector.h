#pragma once

#include <cstdlib>
#include <algorithm>

using namespace std;

template <typename T>
class SimpleVector {
public:
  SimpleVector();
  explicit SimpleVector(size_t size);
  SimpleVector(const SimpleVector& other);
  ~SimpleVector();

  SimpleVector& operator=(const SimpleVector& other);
  T& operator[](size_t index);

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  size_t Size() const;
  size_t Capacity() const;

  void PushBack(const T& value);

private:
  T* data;
  size_t capacity_;
  size_t size_;
};

template <typename T>
SimpleVector<T>::SimpleVector() : data(nullptr), capacity_(0), size_(0) {}

template <typename T>
SimpleVector<T>::SimpleVector(size_t size) : data(new T[size]), capacity_(size), size_(size) {}

template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector<T>& other) : data(new T[other.capacity_]), capacity_(other.capacity_), size_(other.size_) {
  copy(other.begin(), other.end(), begin());
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
  delete[] data;
}

template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(const SimpleVector<T>& other) {
  if (other.size_ <= capacity_) {
    copy(other.begin(), other.end(), begin());
    size_ = other.size_;
  } else {
    // Это так называемая идиома copy-and-swap.
    // Мы создаём временный вектор с помощью
    // конструктора копирования, а затем обмениваем его поля со своими.
    // Так мы достигаем двух целей:
    //  - избегаем дублирования кода в конструкторе копирования
    //    и операторе присваивания
    //  - обеспечиваем согласованное поведение конструктора копирования
    //    и оператора присваивания
    SimpleVector<T> tmp(other);
    swap(tmp.data, data);
    swap(tmp.size_, size_);
    swap(tmp.capacity_, capacity_);
  }
  return *this;
}


template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
  return data[index];
}

template <typename T>
T* SimpleVector<T>::begin() {
  return data;
}

template <typename T>
T* SimpleVector<T>::end() {
  return data + size_;
}

template <typename T>
const T* SimpleVector<T>::begin() const {
  return data;
}

template <typename T>
const T* SimpleVector<T>::end() const {
  return data + size_;
}

template <typename T>
size_t SimpleVector<T>::Size() const {
  return size_;
}

template <typename T>
size_t SimpleVector<T>::Capacity() const {
  return capacity_;
}

template <typename T>
void SimpleVector<T>::PushBack(const T& value) {
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
