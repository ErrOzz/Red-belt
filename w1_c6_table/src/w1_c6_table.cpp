#include "test_runner.h"

#include <vector>
#include <tuple>

using namespace std;

template <typename T>
class Table {
public:
  explicit Table(const size_t& row_num, const size_t& col_num) {
    Resize(row_num, col_num);
  }
  vector<T>& operator [](const size_t& i) {
    return _table.at(i);
  }
  const vector<T>& operator [](const size_t& i) const {
    return _table.at(i);
  }
  void Resize(const size_t& new_row_num, const size_t& new_col_num) {
    _table.resize(new_row_num);
    for (auto& item : _table) {
      item.resize(new_col_num);
    }
  }
  const pair<size_t, size_t> Size() const {
    return {_table.size(), _table.empty() ? 0 : _table[0].size()};
  }

private:
  vector<vector<T>> _table;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
