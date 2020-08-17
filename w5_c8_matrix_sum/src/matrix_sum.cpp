#include "test_runner.h"
#include <vector>
#include <future>
using namespace std;

template <typename Iterator>
int64_t CalculateMatrixSum(Iterator begin, Iterator end) {
  int64_t result = 0;
  for (; begin != end; ++begin) {
    for (const auto& element : *begin) {
      result += element;
    }
  }
  return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  size_t page_size = (matrix.size() + 100) / 4;
  auto begin = matrix.begin();
  vector<future<int64_t>> futures;
  while (begin != matrix.end()) {
    auto end = (matrix.end() - page_size) > begin ? begin + page_size : matrix.end();
    futures.push_back(async([begin, end] () { return CalculateMatrixSum(begin, end); }));
    begin = end;
  }
  int64_t result = 0;
  for (auto& future : futures) {
    result += future.get();
  }
  return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
