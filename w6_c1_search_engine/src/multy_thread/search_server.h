#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <string_view>
#include <mutex>
#include <future>

using namespace std;

template <typename T>
class Synchronized {
public:
  explicit Synchronized(T initial = T()) : value(move(initial)) {}

  struct Access {
    T& ref_to_value;
    lock_guard<mutex> guard;
  };

  Access GetAccess() {
    return {value, lock_guard<mutex>(m)};
  }
private:
  T value;
  mutex m;
};

template <typename T>
using Group = vector<T>;

class InvertedIndex {
public:
  struct Entry {
    size_t docid;
    size_t hitcount;
  };

  InvertedIndex() = default;
  explicit InvertedIndex(istream& document_input);
  const Group<Entry>& Lookup(const string& word) const;
  const deque<string>& GetDocuments() const;

private:
  deque<string> docs;
  map<string, Group<Entry>> index;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input) : sync_index(InvertedIndex(document_input)) {};
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

  static const size_t MAX_DOCS_COUNT = 50000;

 private:
  Synchronized<InvertedIndex> sync_index;
  vector<future<void>> futures;
};
