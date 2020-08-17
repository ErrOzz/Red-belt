#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template <typename T>
class ObjectPool {
public:
  T* Allocate() {
    if (free_.empty()) {
      free_.push(new T);
    }
    auto pObject = free_.front();
    free_.pop();
    busy_.insert(pObject);
    return pObject;
  }
  T* TryAllocate() {
    if (free_.empty()) {
      return nullptr;
    }
    return Allocate();
  }

  void Deallocate(T* object) {
    if (busy_.find(object) == busy_.end()) {
      throw invalid_argument("Not allocated object");
    }
    busy_.erase(object);
    free_.push(object);
  }

  ~ObjectPool() {
    while (!free_.empty()) {
      auto x = free_.front();
      free_.pop();
      delete x;
    }
    for (auto& x : busy_) {
      delete x;
    }
  }

private:
  set<T*> busy_;
  queue<T*> free_;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
