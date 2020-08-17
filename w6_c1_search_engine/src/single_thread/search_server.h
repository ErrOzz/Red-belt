#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

struct Doc_rank {
  size_t docid;
  size_t rank;
};

template <typename T>
using Group = vector<T>;

class InvertedIndex {
public:
  InvertedIndex();
  void Add(string document);
  const vector<Doc_rank>& Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  map<string, Group<Doc_rank>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
