#include "IndexReader.h"
#include <fstream>

IndexReader::IndexReader(const std::string& dir) : dir_(dir) {}

InvertedIndex IndexReader::read(int& corpusSize) {

    InvertedIndex index;

    std::ifstream postingsFile(dir_ + "/postings.bin", std::ios::binary);
    std::ifstream lexiconFile(dir_ + "/lexicon.bin", std::ios::binary);
    std::ifstream docFile(dir_ + "/doc_table.bin", std::ios::binary);
    std::ifstream metaFile(dir_ + "/index.meta");

    std::string line;
    while (std::getline(metaFile, line)) {
        if (line.rfind("corpus_size=", 0) == 0) {
            corpusSize = std::stoi(line.substr(12));
        }
    }

    while (lexiconFile.peek() != EOF) {

        uint32_t termLen;
        lexiconFile.read(reinterpret_cast<char*>(&termLen), sizeof(termLen));

        std::string term(termLen, ' ');
        lexiconFile.read(&term[0], termLen);

        LexiconEntry entry;
        lexiconFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));

        postingsFile.seekg(entry.offset);

        for (uint32_t i = 0; i < entry.count; ++i) {
            Posting p;
            postingsFile.read(reinterpret_cast<char*>(&p), sizeof(p));
            index.addPosting(term, p.docId, p.tf);
        }
    }

    DocID docId;
    int len;
    uint32_t nameLen;
    while (docFile.peek()!=EOF) {
        docFile.read(reinterpret_cast<char*>(&docId),sizeof(docId));
        docFile.read(reinterpret_cast<char*>(&len), sizeof(len));
        docFile.read(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
        std::string name(nameLen,' ');
        docFile.read(&name[0],nameLen);
        index.setDocumentLength(docId, len);
        index.setDocumentName(docId,name);
    }

    return index;
}
