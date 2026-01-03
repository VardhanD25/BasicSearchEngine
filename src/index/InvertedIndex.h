#pragma once
#include <string>
#include <unordered_map>
#include <vector>

using DocID = int;

class InvertedIndex {
public:
    void addDocument(DocID docId, const std::string& filename, const std::vector<std::string>& tokens);

    bool contains(const std::string& term) const;
    const std::vector<DocID>& getPostings(const std::string& term) const;
    int getTermFrequency(const std::string& term, DocID docId) const;
    int getDocumentLength(DocID docId) const;

    // Used by IndexWriter
    const std::unordered_map<std::string, std::vector<DocID>>& getIndex() const;
    const std::unordered_map<std::string,
          std::unordered_map<DocID, int>>& getTermFrequencies() const;
    const std::unordered_map<DocID, int>& getDocumentLengths() const;
    std::string getDocumentName(DocID docId) const;
    const std::unordered_map<DocID, std::string>& getDocFilenames() const;

    // Used by IndexReader
    void addPosting(const std::string& term, DocID docId, int tf);
    void setDocumentLength(DocID docId, int length);
    void setDocumentName(DocID docId, std::string name);

private:
    std::unordered_map<std::string, std::vector<DocID>> index_;
    std::unordered_map<std::string,
        std::unordered_map<DocID, int>> termFrequencies_;
    std::unordered_map<DocID, int> documentLengths_;
    std::unordered_map<DocID,std::string> docFilenames_;
};
