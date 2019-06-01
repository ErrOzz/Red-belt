#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <string_view>
#include <future>
#include <functional>
#include <iterator>

using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other);
};

void Stats::operator +=(const Stats& other) {
  for (const auto& item : other.word_frequences) {
    word_frequences[item.first] += item.second;
  }
}

Stats ExploreLine(const set<string>& key_words, const string& line) {
  Stats result;
  istringstream line_splitter(line);
  for (string& word : vector<string>{istream_iterator<string>(line_splitter), istream_iterator<string>()}) {
    if (key_words.count(word)) {
      result.word_frequences[word]++;
    }
  }
  return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

vector<stringstream> Stream_splitter(istream& input) {
  const size_t batch_size = 3000;
  vector<stringstream> streams;
  stringstream stream;
  bool end = false;
  while (!end) {
    for (size_t i = 0; i < batch_size; ++i) {
      string line;
      if (getline(input, line)) {
        stream << line << '\n';
      } else {
        end = true;
        break;
      }
    }
    streams.push_back(move(stream));
    stream.clear();
    if (end) { break; }
  }
  return streams;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  vector<stringstream> streams = Stream_splitter(input);
  vector<future<Stats>> futures;
  for (auto& stream : streams) {
    futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(stream)));
  }
  Stats result;
  for (auto& _future : futures) {
    result += _future.get();
  }
  return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
