#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    Id id;
    ++current_number;
    if(unused_id.empty()) {
      collection.push_back({{move(object), 0}, current_number});
      id = collection.size() - 1;
    } else {
      id = *unused_id.begin();
      unused_id.erase(unused_id.begin());
      collection[id] = {{move(object), 0}, current_number};
    }
    Priority_at(0).insert({current_number, id});
    return id;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for(; range_begin != range_end; ++range_begin) {
      *ids_begin++ = Add(move(*range_begin));
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    if(id < collection.size() && !unused_id.count(id)) {
      return true;
    }
    return false;
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return collection[id].object_pair.first;
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    priority_t& id_priority = collection[id].object_pair.second;
    order_t& order_number = collection[id].order_number;
    priority[id_priority].erase({order_number, id});
    Priority_at(++id_priority).insert({order_number, id});
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    Id max_object_id = prev(priority.back().end())->second;
    return collection[max_object_id].object_pair;
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    set<pair<order_t, Id>>& max_priority_set = priority.back();
    Id max_object_id = prev(max_priority_set.end())->second;
    unused_id.insert(max_object_id);
    max_priority_set.erase(prev(max_priority_set.end()));
    while(priority.back().empty()) {
      priority.pop_back();
    }

    return move(collection[max_object_id].object_pair);
  }

private:
  using priority_t = int;
  using order_t = size_t;

  struct PriorityObject {
    pair<T, priority_t> object_pair;
    order_t order_number;
  };

  order_t current_number = 0;

  vector<PriorityObject> collection;
  set<Id> unused_id;                        // Необходимо для повторного использования освободившихся идентификаторов
  vector<set<pair<order_t, Id>>> priority;  //Для поиска последнего элемента с максимальным приоритетом

  set<pair<order_t, Id>>& Priority_at(priority_t p) {
    if(static_cast<size_t>(p) >= priority.size()) {
      priority.push_back(set<pair<order_t, Id>>{});
    }
    return priority[p];
  }
};

class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  StringNonCopyable s = "black", t = "blue", r = "cian";
  vector<StringNonCopyable> str;
  str.push_back(move(s));
  str.push_back(move(t));
  str.push_back(move(r));
  vector<size_t> ids(3);

  strings.Add(str.begin(), str.end(), ids.begin());
  {
    ASSERT_EQUAL(ids, vector<size_t>({3, 4, 5}));
  }
  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    pair<const string&, int> item = strings.GetMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "cian");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
