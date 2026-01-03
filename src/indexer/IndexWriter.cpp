#include "IndexWriter.h"
#include <fstream>

IndexWriter::IndexWriter(const std::string& dir) : dir_(dir) {}

void IndexWriter::write(const InvertedIndex& index, int corpusSize) {

    std::ofstream postingsFile(dir_ + "/postings.bin", std::ios::binary);
    std::ofstream lexiconFile(dir_ + "/lexicon.bin", std::ios::binary);
    std::ofstream docFile(dir_ + "/doc_table.bin", std::ios::binary);
    std::ofstream metaFile(dir_ + "/index.meta");

    uint64_t offset = 0;

    for (const auto& [term, docIds] : index.getIndex()) {

        uint32_t termLen = term.size();
        lexiconFile.write(reinterpret_cast<char*>(&termLen), sizeof(termLen));
        lexiconFile.write(term.data(), termLen);

        LexiconEntry entry{offset, static_cast<uint32_t>(docIds.size())};
        lexiconFile.write(reinterpret_cast<char*>(&entry), sizeof(entry));

        for (DocID docId : docIds) {
            uint32_t tf = index.getTermFrequency(term, docId);
            Posting p{static_cast<uint32_t>(docId), tf};
            postingsFile.write(reinterpret_cast<char*>(&p), sizeof(p));
            offset += sizeof(p);
        }
    }

    for (const auto& [docId, len] : index.getDocumentLengths()) {
        docFile.write(reinterpret_cast<const char*>(&docId), sizeof(docId));
        docFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
        std::string name=index.getDocumentName(docId);
        uint32_t nameLen=name.size();

        docFile.write(reinterpret_cast<const char*>(&nameLen),sizeof(nameLen));
        docFile.write(name.data(),nameLen);
    }

    metaFile << "corpus_size=" << corpusSize << "\n";
    metaFile << "version=1\n";
}
