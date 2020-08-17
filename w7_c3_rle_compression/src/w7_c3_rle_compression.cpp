#include "profile.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>

using namespace std;

unsigned int Substr_size(string& line, size_t l, size_t r) {
  istringstream iss(line);
  unsigned int result = 0;
  size_t ch_count;
  bool finish = false;
  while (!finish) {
    if (isdigit(iss.peek())) {
      iss >> ch_count;
    } else if (iss.peek() != '\n') {
      ch_count = 1;
    } else {
      break;
    }
    iss.ignore();
    size_t ch_count_result = 0;
    if (l > ch_count) {
      l -= ch_count;
      r -= ch_count;
    } else if (l > 1) {
      ch_count_result = ch_count - l + 1;
      if (r > ch_count) {
        r -= ch_count;
      } else {
        ch_count_result = r - l + 1;
        finish = true;
      }
      l = 1;
    } else if (r > ch_count) {
      r -= ch_count;
      ch_count_result = ch_count;
    } else {
      ch_count_result = r;
      finish = true;
    }
    result += ch_count_result > 1 ? to_string(ch_count_result).size() + 1 : ch_count_result == 1 ? 1 : 0;
  }
  return result;
}

int main() {
  LOG_DURATION("Total")
//  ios::sync_with_stdio(false);
//  cin.tie(nullptr);
  string line;
  getline(cin, line);
  unsigned int q;
  cin >> q;
  size_t l, r;
  for (size_t i = 0; i < q; ++i) {
    cin >> l >> r;
    cout << Substr_size(line, l, r) << '\n';
  }
  return 0;
}
