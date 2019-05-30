#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <memory>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

// Теоретически, нулевое количество копирований, однако значительно затратнее по времени...
//
//template <typename It>
//class Char_ptr {
//private:
//  const shared_ptr<Char<It>> ptr;
//public:
//  explicit Char_ptr(const It it) : ptr(make_shared<Char<It>>(*it)) {};
//  shared_ptr<Char<It>> Get_ptr() const {
//    return ptr;
//  }
//  bool operator<(Char_ptr item) const {
//    return *ptr < *item.Get_ptr();
//  }
//  bool operator==(Char_ptr item) const {
//    return *ptr == *item.Get_ptr();
//  }
//};
//
//template <typename It>
//set<Char_ptr<It>> Uniq_set(It first, It last) {
//  set<Char_ptr<It>> result;
//  for(; first != last; ++first) {
//    result.insert(Char_ptr<It>(first));
//  }
//  return result;
//}
//
//template <typename String>
//using Key = set<Char_ptr<typename String::iterator>>;

template <typename String>
using Key = set<Char<String>>;

template <typename String>
Key<String> Uniq_set(const String& string) {
  Key<String> result;
  for(const auto& c : string) {
    result.insert(c);
  }
  return result;
}

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
  map<Key<String>, Group<String>> grouped;
  for(auto& string : strings) {
    grouped[Uniq_set(string)].push_back(move(string));
  }
  vector<Group<String>> result;
  for(auto& group : grouped) {
    result.push_back(move(group.second));
  }
  return result;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2u);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4u);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
