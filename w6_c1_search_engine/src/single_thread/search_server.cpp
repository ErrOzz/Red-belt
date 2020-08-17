#include "profile.h"

#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;
  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
  index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  vector<size_t> docid_rank(50000, 0);
  Group<Doc_rank> search_results;
  search_results.reserve(50000);

  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    for (const auto& word : words) {
      for (const auto& doc_hit : index.Lookup(word)) {
        docid_rank[doc_hit.docid] += doc_hit.rank;
      }
    }

    search_results.clear();
    for (size_t idx = 0; idx < docid_rank.size(); ++idx) {
      if (docid_rank[idx] != 0) {
        search_results.push_back({idx, docid_rank[idx]});
        docid_rank[idx] = 0;
      }
    }

    auto middle_it = search_results.size() > 5 ? begin(search_results) + 5 : end(search_results);
    partial_sort(begin(search_results), middle_it, end(search_results),
      [](Doc_rank lhs, Doc_rank rhs) {
        int lhs_docid = lhs.docid;
        int lhs_hit_count = lhs.rank;
        int rhs_docid = rhs.docid;
        int rhs_hit_count = rhs.rank;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}

InvertedIndex::InvertedIndex() {
  docs.reserve(50000);
}

void InvertedIndex::Add(string document) {
  docs.push_back(move(document));
  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(docs.back())) {
    Group<Doc_rank>& word_docs = index[word];
    if (word_docs.empty()) {
      word_docs.push_back({docid, 1});
    } else if (word_docs.back().docid == docid) {
      ++word_docs.back().rank;
    } else {
      word_docs.push_back({docid, 1});
    }
  }
}

const Group<Doc_rank>& InvertedIndex::Lookup(const string& word) const {
  static const Group<Doc_rank> empty;
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty;
  }
}
