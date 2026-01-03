#include "Indexer.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

Indexer::Indexer(InvertedIndex& index)
    : index_(index) {}

void Indexer::indexDirectory(const std::string& directoryPath) {
    if (!fs::exists(directoryPath)) {
        throw std::runtime_error("Directory does not exist: " + directoryPath);
    }   
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            indexFile(entry.path().string());
        }
    }
}

void Indexer::indexFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    std::stringstream buffer;
    buffer << file.rdbuf();

    auto tokens = tokenizer_.tokenize(buffer.str());

    DocID docId = nextDocId_++;
    index_.addDocument(docId, filePath, tokens);

    documentStore_[docId] = filePath;
}

const std::unordered_map<DocID, std::string>&
Indexer::getDocumentStore() const {
    return documentStore_;
}
