#include "profile.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    int newWords = 0;
    for (const auto& word : words) {
      if (dict.insert(word).second) {
        ++newWords;
      }
    }
    return newWords;
  }

  vector<string> KnownWords() {
    return {dict.begin(), dict.end()};
  }
};

int main() {
  LOG_DURATION("Total")
  Learner learner;
  ifstream fs("src/words.txt");
  string line;
  while (getline(fs, line)) {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    cout << learner.Learn(words) << "\n";
  }
  fs.close();
  cout << "=== known words ===\n";
  for (auto word : learner.KnownWords()) {
    cout << word << "\n";
  }
}
