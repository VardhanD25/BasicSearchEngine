#pragma once

#include <string>
#include <vector>
#include <utility>
#include "InvertedIndex.h"
#include "Tokenizer.h"

struct SearchResult {
    DocID docId;
    double score;
    std::string filename; // <--- The user wants this!
};

class QueryEngine {
public:
    QueryEngine(const InvertedIndex& index, int corpusSize);

    // Existing boolean search
    std::vector<DocID> searchBoolean(const std::string& query) const;

    // New ranked search
    std::vector<SearchResult> searchRanked(const std::string& query) const;

private:
    const InvertedIndex& index_;
    Tokenizer tokenizer_;
    int corpusSize_;

    std::vector<DocID> andMerge(const std::vector<std::vector<DocID>>& postings) const;
    std::vector<DocID> orMerge(const std::vector<std::vector<DocID>>& postings) const;

    double computeIDF(const std::string& term) const;
};
