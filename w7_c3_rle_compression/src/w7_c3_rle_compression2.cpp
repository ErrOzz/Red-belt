#include "profile.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>
#include <cstdint>

using namespace std;

uint32_t Char_size_compressed(unsigned int& uncomp_size) {
  return uncomp_size > 1 ? to_string(uncomp_size).size() + 1 : 1;
}

uint32_t Get_int(istream& is) {
  uint32_t number;
  if (isdigit(is.peek())) {
    is >> number;
  } else if (is.peek() != '\n') {
    number = 1;
  } else {
    number = 0;
  }
  return number;
}

struct Substr {
  size_t begin;
  size_t end;
  string str;

  size_t Last() const {
    return end - begin + 1;
  }

  bool operator<(const Substr& other) const {
    return end < other.end;
  }
  bool operator<(const size_t& other_end) const {
    return end < other_end;
  }
};



set<Substr> Parse_line(istream& is) {
  set<Substr> result;
  size_t begin = 1;
  size_t end = 0;
  string str;
  str.reserve(110);
  uint32_t number = 1;
  while (number) {
    number = Get_int(is);
    if (number) {
      end += number;
      str += number > 1 ? to_string(number) : "";
      char ch;
      is >> ch;
      str += ch;
    }
    if (str.size() > 100 || !number) {
      result.insert({begin, end, move(str)});
      begin = end + 1;
      str.clear();
    }
  }
  return result;
}

uint32_t Substr_size(const string& line, size_t l, size_t r) {
  istringstream iss(line);
  uint32_t result = 0;
  size_t ch_count = 1;
  while (ch_count) {
    ch_count = Get_int(iss);
    if (!ch_count) { break; }
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
        ch_count = 0;
      }
      l = 1;
    } else if (r > ch_count) {
      r -= ch_count;
      ch_count_result = ch_count;
    } else {
      ch_count_result = r;
      ch_count = 0;
    }
    result += ch_count_result > 1 ? to_string(ch_count_result).size() + 1 : ch_count_result == 1 ? 1 : 0;
  }
  return result;
}

int main() {
  LOG_DURATION("Total");
//  ios::sync_with_stdio(false);
//  cin.tie(nullptr);

  auto string_set(Parse_line(cin));
  unsigned int q;
  cin >> q;
  vector<pair<size_t, size_t>> qry;
  qry.reserve(100000);
  for (unsigned int i = 0; i < q; ++i) {
    size_t l, r;
    cin >> l >> r;
    qry.push_back({move(l), move(r)});
  }
  uint32_t result;
  for (const auto& lr : qry) {
    auto begin_it = string_set.lower_bound({0u, lr.first});
    auto preend_it = string_set.lower_bound({0u, lr.second});
    size_t l = lr.first - begin_it->begin + 1;
    size_t r = lr.second - preend_it->begin + 1;
    if (begin_it == preend_it) {
      result  = Substr_size(begin_it->str, l, r);
    } else {
      result = Substr_size(begin_it->str, l, begin_it->Last());
      result += Substr_size(preend_it->str, 1, r);
      for (auto it = next(begin_it); it != preend_it; ++it) {
        result += it->str.size();
      }
    }
    cout << result << '\n';
  }
  return 0;
}
