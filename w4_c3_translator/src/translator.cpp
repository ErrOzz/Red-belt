#include "test_runner.h"
#include <string>
#include <string_view>
#include <set>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target) {
    const string_view source_ref = GetReference(forward_dict_, backward_dict_, source);
    const string_view target_ref = GetReference(backward_dict_, forward_dict_, target);
    forward_dict_[source_ref] = target_ref;
    backward_dict_[target_ref] = source_ref;
  }
  string_view TranslateForward(string_view source) const {
    return Translate(forward_dict_, source);
  }
  string_view TranslateBackward(string_view target) const {
    return Translate(backward_dict_, target);
  }

private:
  string_view GetReference (map<string_view, string_view>& dict,
                            map<string_view, string_view>& rev_dict,
                            string_view word) {
    auto find_it = dict.find(word);
    if (find_it == dict.end()) {
      return *(words_.emplace(word).first);
    }
    rev_dict.erase(find_it->second);
    return find_it->first;
  }
  string_view Translate(const map<string_view, string_view>& dict, string_view word) const {
    auto result_it = dict.find(word);
    return result_it == dict.end() ? "" : result_it->second;
  }

  set<string> words_;
  map<string_view, string_view> forward_dict_, backward_dict_;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));
  translator.Add(string("super"), string("super"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
  ASSERT_EQUAL(translator.TranslateForward("super"), "super");
  ASSERT_EQUAL(translator.TranslateBackward("super"), "super");

  translator.Add(string("stol"), string("desk"));
  ASSERT_EQUAL(translator.TranslateForward("stol"), "desk");
  translator.Add(string("stol"), string("desk"));
  ASSERT_EQUAL(translator.TranslateBackward("desk"), "stol");

  translator.Add(string("vitrina"), string("window"));
  ASSERT_EQUAL(translator.TranslateForward("vitrina"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("window"), "vitrina");

  translator.Add(string("stol"), string("window"));
  ASSERT_EQUAL(translator.TranslateForward("stol"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("window"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
