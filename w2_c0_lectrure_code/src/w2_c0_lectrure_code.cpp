#include "profile.h"

#include <vector>

using namespace std;

vector<string> GenerateBigVector() {
  vector<string> result;
  {
    LOG_DURATION("Vector generation");
    for (int i = 0; i < 28000; ++i) {
      LOG_DURATION("Iter" + to_string(i))
      result.push_back(to_string(i));
    }
  }
  return result;
}

int main() {
  LOG_DURATION("Total");
  cout << GenerateBigVector().size() << endl;
  return 0;
}
