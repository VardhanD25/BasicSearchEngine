#include "QueryEngine.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <iterator>

QueryEngine::QueryEngine(const InvertedIndex& index, int corpusSize)
    : index_(index), corpusSize_(corpusSize) {}

double QueryEngine::computeIDF(const std::string& term) const {
    int df = index_.getPostings(term).size();
    return std::log((corpusSize_ + 1.0) / (df + 1.0)) + 1.0;
}


std::vector<DocID> QueryEngine::searchBoolean(const std::string& query) const {
    auto tokens = tokenizer_.tokenize(query);
    if (tokens.empty()) return {};

    bool isOrQuery = false;
    std::vector<std::string> terms;

    for (const auto& t : tokens) {
        if (t == "or") isOrQuery = true;
        else terms.push_back(t);
    }

    std::vector<std::vector<DocID>> postings;
    for (const auto& term : terms) {
        postings.push_back(index_.getPostings(term));
    }

    return isOrQuery ? orMerge(postings) : andMerge(postings);
}

std::vector<SearchResult> QueryEngine::searchRanked(const std::string& query) const {

    auto terms = tokenizer_.tokenize(query);
    if (terms.empty()) return {};

    // 1. Calculate Query TF (Term Frequency)
    std::unordered_map<std::string, int> queryTF;
    for (const auto& t : terms) queryTF[t]++;

    std::unordered_map<DocID, double> scores;

    // 2. Accumulate Scores (Raw TF-IDF)
    for(const auto& [term, qtf] : queryTF) {
        if (!index_.contains(term)) continue;

        // IDF: Measures how rare the term is in the whole corpus
        double idf = computeIDF(term);
        
        // Weight of term in Query
        double w_tq = qtf * idf;

        for (DocID docId : index_.getPostings(term)) {
            // TF: How often term appears in this specific document
            int tf_td = index_.getTermFrequency(term, docId);
            
            // Weight of term in Document
            double w_td = tf_td * idf;

            // Dot Product (Accumulate score)
            scores[docId] += w_td * w_tq;
        }
    }

    // 3. Create Results
    std::vector<SearchResult> results;
    for (auto& [docId, score] : scores) {
        // OPTIONAL: Simple Length Normalization
        // Divide by document length to prevent long documents from dominating just because they are long.
        // We add 1.0 to avoid division by zero or overly aggressive penalization.
        double lengthNorm = index_.getDocumentLength(docId) + 1.0; 
        
        // Final Score
        double finalScore = score / std::log(lengthNorm); 

        results.push_back({docId, finalScore, index_.getDocumentName(docId)});
    }

    // 4. Sort (Highest score first)
    std::sort(results.begin(), results.end(),
              [](const SearchResult& a, const SearchResult& b) { 
                  return a.score > b.score; 
              });

    return results;
}


std::vector<DocID> QueryEngine::andMerge(
    const std::vector<std::vector<DocID>>& postings) const {

    if (postings.empty()) return {};

    std::vector<DocID> result = postings[0];

    for (size_t i = 1; i < postings.size(); ++i) {
        std::vector<DocID> temp;
        std::set_intersection(
            result.begin(), result.end(),
            postings[i].begin(), postings[i].end(),
            std::back_inserter(temp)
        );
        result = std::move(temp);

        if (result.empty()) break;
    }

    return result;
}
std::vector<DocID> QueryEngine::orMerge(
    const std::vector<std::vector<DocID>>& postings) const {

    std::vector<DocID> result;

    for (const auto& list : postings) {
        result.insert(result.end(), list.begin(), list.end());
    }

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}
