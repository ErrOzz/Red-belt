#include "profile.h"

#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

InvertedIndex::InvertedIndex(istream& document_input) {
  for (string current_document; getline(document_input, current_document); ) {
    docs.push_back(move(current_document));
    const size_t id = docs.size() - 1;
    for (const auto& word : SplitIntoWords(docs[id])) {
      Group<Entry>& hitcounts = index[word];
      if (hitcounts.empty() || hitcounts.back().docid != id) {
        hitcounts.push_back({id, 1});
      } else {
        ++hitcounts.back().hitcount;
      }
    }
  }
}

const Group<InvertedIndex::Entry>& InvertedIndex::Lookup(const string& word) const {
  static const Group<Entry> empty;
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty;
  }
}

const deque<string>& InvertedIndex::GetDocuments() const {
  return docs;
}

void UpdateIndex(istream& document_input, Synchronized<InvertedIndex>& index) {
  index.GetAccess().ref_to_value = InvertedIndex(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  futures.push_back(async(UpdateIndex, ref(document_input), ref(sync_index)));
}

void AddQueriesStream_SingleThread(istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& sync_index) {
  vector<size_t> docid_rank(SearchServer::MAX_DOCS_COUNT, 0);
  Group<Entry> search_results;
  search_results.reserve(SearchServer::MAX_DOCS_COUNT);

  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);

    for (const auto& word : words) {
      for (const auto& doc_hit : sync_index.GetAccess().ref_to_value.Lookup(word)) {
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

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  futures.GetAccess().ref_to_value.push_back(async(AddQueriesStream_SingleThread, ref(query_input), ref(search_results_output), ref(sync_index)));
}
