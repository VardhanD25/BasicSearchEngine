#include "InvertedIndex.h"

void InvertedIndex::addDocument(DocID docId, const std::string& filename, const std::vector<std::string>& tokens) {

    documentLengths_[docId] = tokens.size();
    docFilenames_[docId]=filename;

    for (const auto& token : tokens) {
        termFrequencies_[token][docId]++;

        auto& postings = index_[token];
        if (postings.empty() || postings.back() != docId) {
            postings.push_back(docId);
        }
    }
}

std::string InvertedIndex::getDocumentName(DocID docId)const{
    auto it=docFilenames_.find(docId);
    return (it!=docFilenames_.end())?it->second:"";
}

const std::unordered_map<DocID, std::string>& InvertedIndex::getDocFilenames()const{
    return docFilenames_;
}

bool InvertedIndex::contains(const std::string& term) const {
    return index_.find(term) != index_.end();
}

const std::vector<DocID>&
InvertedIndex::getPostings(const std::string& term) const {
    static const std::vector<DocID> empty;
    auto it = index_.find(term);
    return (it != index_.end()) ? it->second : empty;
}

int InvertedIndex::getTermFrequency(const std::string& term,
                                   DocID docId) const {
    auto tIt = termFrequencies_.find(term);
    if (tIt == termFrequencies_.end()) return 0;

    auto dIt = tIt->second.find(docId);
    return (dIt != tIt->second.end()) ? dIt->second : 0;
}

int InvertedIndex::getDocumentLength(DocID docId) const {
    auto it = documentLengths_.find(docId);
    return (it != documentLengths_.end()) ? it->second : 0;
}

const std::unordered_map<std::string, std::vector<DocID>>& InvertedIndex::getIndex() const { return index_; }
const std::unordered_map<std::string, std::unordered_map<DocID, int>>& InvertedIndex::getTermFrequencies() const {
    return termFrequencies_;
}
const std::unordered_map<DocID, int>& InvertedIndex::getDocumentLengths() const {
    return documentLengths_;
}

void InvertedIndex::addPosting(const std::string& term,
                               DocID docId, int tf) {
    index_[term].push_back(docId);
    termFrequencies_[term][docId] = tf;
}

void InvertedIndex::setDocumentLength(DocID docId, int length) {
    documentLengths_[docId] = length;
}

void InvertedIndex::setDocumentName(DocID docId, std::string name){
    docFilenames_[docId]=name;
}