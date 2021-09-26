#pragma once
#include "synchronized.h"

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

class InvertedIndex {
public:
    InvertedIndex() {
        docs.reserve(50000);
    }

    size_t GetDocsCount();

    void Add(string document);

    const vector<pair<size_t, size_t>>& Lookup(const string_view &word_view) const;

private:
    unordered_map<string_view, vector<pair<size_t, size_t>>> index;
    vector<string>docs;

    vector<pair<size_t, size_t>> empty_;
};

class SearchServer {
public:
    SearchServer() = default;

    explicit SearchServer(istream &document_input);

    void UpdateDocumentBase(istream &document_input);

    void AddQueriesStream(istream &query_input, ostream &search_results_output);

    ~SearchServer() {
        for (auto& future: futures) {
            future.get();
        }
    }

private:
    Synchronized<InvertedIndex> index;
    vector<future<void>>futures;

    void AddQueriesStreamSingleThread(istream &query_input, ostream &search_results_output);
};