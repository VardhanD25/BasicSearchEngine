#include "InvertedIndex.h"
#include "Indexer.h"
#include "QueryEngine.h"
#include "IndexWriter.h"
#include "IndexReader.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Configuration
    std::string dataDir = "../data"; // Directory containing .txt files
    std::string indexDir="../index_data";
    bool rebuildIndex = true;        // SET TO TRUE since file format changed!

    // ==========================================
    // PHASE 1: Indexing & Writing (The "Crawler" part)
    // ==========================================
    if (rebuildIndex) {
        std::cout << "--- Indexing Mode ---\n";
        
        InvertedIndex index;
        Indexer indexer(index);

        try {
            std::cout << "Scanning directory: " << dataDir << "...\n";
            indexer.indexDirectory(dataDir);
        } catch (const std::exception& e) {
            std::cerr << "Indexing failed: " << e.what() << "\n";
            return 1;
        }

        // Get corpus size from the index itself now
        int corpusSize = index.getDocFilenames().size(); 

        if (corpusSize == 0) {
            std::cerr << "No documents found to index! Check your path.\n";
            return 1;
        }

        IndexWriter writer(indexDir);
        writer.write(index, corpusSize);

        std::cout << "Successfully indexed " << corpusSize << " documents.\n";
        std::cout << "Index persisted to disk.\n\n";
    }

    // ==========================================
    // PHASE 2: Loading & Searching (The "Server" part)
    // ==========================================
    std::cout << "--- Search Mode ---\n";

    // 1. Load the index from disk
    int loadedCorpusSize = 0;
    IndexReader reader(indexDir);
    
    // This will now read the new format with filenames
    InvertedIndex loadedIndex = reader.read(loadedCorpusSize); 

    std::cout << "Index loaded. Corpus size: " << loadedCorpusSize << "\n";

    // 2. Initialize Query Engine
    QueryEngine engine(loadedIndex, loadedCorpusSize);

    // 3. Interactive Search Loop
    while (true) {
        std::string query;
        std::cout << "\nEnter query (or 'exit'): ";
        std::getline(std::cin, query);

        if (query == "exit") break;
        if (query.empty()) continue;

        auto results = engine.searchRanked(query);

        if (results.empty()) {
            std::cout << "No results found.\n";
        } else {
            std::cout << "Found " << results.size() << " results:\n";
            int rank = 1;
            for (const auto& [docId,score,name] : results) {
                // Retrieve the filename from the loaded index
                //std::string filename = loadedIndex.getDocumentName(docId);
                std::string filename=name;
                // Optional: Cleanup path to just show filename
                std::string simpleName = fs::path(filename).filename().string();

                std::cout << "#" << rank++ << " " 
                          << "[" << simpleName << "] "
                          << "(Score: " << score << ")\n";
                
                // Limit display to top 5
                if (rank > 10) break;
            }
        }
    }

    return 0;
}