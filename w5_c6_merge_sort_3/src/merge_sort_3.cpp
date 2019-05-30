#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
using value_t = typename RandomIt::value_type;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  const size_t range_length = range_end - range_begin;
  if(range_length < 2) { return; }
  vector<value_t<RandomIt>> values(make_move_iterator(range_begin), make_move_iterator(range_end));
  const size_t distance_third = range_length / 3;
  const RandomIt one_third = values.begin() + distance_third;
  const RandomIt two_third = one_third + distance_third;
  MergeSort(values.begin(), one_third);
  MergeSort(one_third, two_third);
  MergeSort(two_third, values.end());
  vector<value_t<RandomIt>> tmp;
  merge(make_move_iterator(values.begin()), make_move_iterator(one_third),
        make_move_iterator(one_third), make_move_iterator(two_third),
        back_inserter(tmp));
  merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
        make_move_iterator(two_third), make_move_iterator(values.end()),
        range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  for(const auto& n : numbers) {
    cout << n << ' ';
  }
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
