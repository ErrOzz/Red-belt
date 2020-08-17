//#include "test_runner.h"

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template <typename T>
class Deque {
private:
  vector<T> fnt, bck;

  void CheckIndex(const size_t& index) const {
    if (index >= Size() || index < 0) {
      throw out_of_range("Index is out of range");
    }
  }

public:
  explicit Deque() = default;

  const bool Empty() const {
    return fnt.empty() && bck.empty();
  }

  void Clear() {
    fnt.clear();
    bck.clear();
  }

  const size_t Size() const {
    return fnt.size() + bck.size();
  }

  T& operator[](size_t index) {
    return index < fnt.size() ? fnt[fnt.size() - 1 - index] : bck[index - fnt.size()];
  }

  const T& operator[](size_t index) const {
    return index < fnt.size() ? fnt[fnt.size() - 1 - index] : bck[index - fnt.size()];
  }

  T& At(size_t index) {
    CheckIndex(index);
    return (*this)[index];
  }

  const T& At(size_t index) const {
    CheckIndex(index);
    return (*this)[index];
  }

  T& Front() {
    return At(0);
  }

  const T& Front() const {
    return At(0);
  }

  T& Back() {
    return At(Size() - 1);
  }

  const T& Back() const {
    return At(Size() - 1);
  }

  void PushFront(const T& item) {
    fnt.push_back(item);
  }

  void PushBack(const T& item) {
    bck.push_back(item);
  }
};
/*
void TestDeque() {
  Deque<int> dq;
  ASSERT_EQUAL(dq.Size(), 0u);
  ASSERT_EQUAL(dq.Empty(), true);
  try {
    dq.Front();
  } catch (exception& e) {
    cerr << e.what() << endl;
  }
  dq.PushFront(5);
  dq.PushBack(-5);
  ASSERT_EQUAL(dq.Size(), 2u);
  ASSERT_EQUAL(dq[0], 5);
  ASSERT_EQUAL(dq[1], -5);
  const int a = dq.Front();
  ASSERT_EQUAL(a, 5);
  dq[0] = 6;
  dq[1] = -6;
  ASSERT_EQUAL(dq.At(0), 6);
  ASSERT_EQUAL(dq.At(1), -6);
  dq.At(0) = 9;
  dq.At(1) = -9;
  ASSERT_EQUAL(dq.At(0), 9);
  ASSERT_EQUAL(dq.At(1), -9);
  dq.PushFront(5);
  dq.PushBack(-5);
  ASSERT_EQUAL(dq.Front(), 5);
  ASSERT_EQUAL(dq.Back(), -5);
  ASSERT_EQUAL(dq[1], 9);
  ASSERT_EQUAL(dq[2], -9);
  ASSERT_EQUAL(dq.Size(), 4u);
  dq.PushFront(10);
  dq.PushFront(15);
  ASSERT_EQUAL(dq.Front(), 15);
}
*/
int main() {
//  TestRunner tr;
//  RUN_TEST(tr, TestDeque);
  return 0;
}
