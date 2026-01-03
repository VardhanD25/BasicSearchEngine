#pragma once

#include <string>
#include <unordered_map>
#include "InvertedIndex.h"
#include "Tokenizer.h"

class Indexer {
public:
    explicit Indexer(InvertedIndex& index);

    void indexDirectory(const std::string& directoryPath);

    const std::unordered_map<DocID, std::string>& getDocumentStore() const;

private:
    InvertedIndex& index_;
    Tokenizer tokenizer_;
    DocID nextDocId_ = 1;

    std::unordered_map<DocID, std::string> documentStore_;

    void indexFile(const std::string& filePath);
};
