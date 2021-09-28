#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>

unordered_map<string_view, size_t> SplitIntoWords(string_view line_view) {
    unordered_map<string_view, size_t> wordcount;

    size_t current_word_start = 0;
    size_t length = 0;
    for (size_t i = 0; i < line_view.length(); i++) {
        if (!isspace(line_view[i])) {
            length++;
        } else {
            if (length) wordcount[line_view.substr(current_word_start, length)]++;
            current_word_start = i + 1;
            length = 0;
        }
    }
    if (length) wordcount[line_view.substr(current_word_start, length)]++;
    return wordcount;
}

SearchServer::SearchServer(istream &document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document);) {
        new_index.Add(move(current_document));
    }

    swap(index.GetAccess().ref_to_value, new_index);
}

void SearchServer::AddQueriesStream(
        istream &query_input, ostream &search_results_output
) {
    futures.push_back(async(&SearchServer::AddQueriesStreamSingleThread, this, ref(query_input), ref(search_results_output)));
}

void SearchServer::AddQueriesStreamSingleThread(istream &query_input, ostream &search_results_output) {
    TotalDuration dur("words");
    TotalDuration whole("whole");
    TotalDuration sort("sort");
    size_t documents_number = index.GetAccess().ref_to_value.GetDocsCount();
    vector<uint32_t>docid_count(documents_number);
    vector<size_t>docids(documents_number);
    iota(docids.begin(), docids.end(), 0);

    for (string current_query; getline(query_input, current_query);) {
        ADD_DURATION(whole);
        unordered_map<size_t, size_t> search_results;
        const auto words = SplitIntoWords(current_query);
        docid_count.assign(docid_count.size(), 0);
        for (const auto &[word, wordcount]: words) {
            ADD_DURATION(dur);
            for (const auto &[docid, hitcount]: index.GetAccess().ref_to_value.Lookup(word)) {
                docid_count[docid] += hitcount * wordcount;
            }
        }
        iota(docids.begin(), docids.end(), 0);
        size_t shift = docids.size() < 5 ? docids.size() : 5;
        {
            ADD_DURATION(sort);
            partial_sort(begin(docids), begin(docids) + shift, end(docids),
                         [&docid_count](size_t lhs, size_t rhs) {
                             if (docid_count[lhs] != docid_count[rhs]) return docid_count[lhs] > docid_count[rhs];
                             return lhs < rhs;
                         });
        }
        search_results_output << current_query << ':';
        for (size_t docid: Head(docids, shift)) {
            if (!docid_count[docid]) break;
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << docid_count[docid] << '}';
        }
        search_results_output << endl;
    }
}

void InvertedIndex::Add(string document) {
    docs.push_back(move(document));
    const auto docid = docs.size() - 1;
    for (const auto &[word, wordcount]: SplitIntoWords(docs.back())) {
        index[word].push_back({docid, wordcount});
    }
}

const vector<pair<size_t, size_t>> &InvertedIndex::Lookup(string_view word_view) const {
    if (auto it = index.find(word_view); it != index.end()) {
        return it->second;
    } else {
        return empty_;
    }
}

size_t InvertedIndex::GetDocsCount() {
    return docs.size();
}
