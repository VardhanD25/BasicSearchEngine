#pragma once
#include "InvertedIndex.h"
#include "IndexIO.h"
#include <string>

class IndexWriter {
public:
    explicit IndexWriter(const std::string& dir);
    void write(const InvertedIndex& index, int corpusSize);

private:
    std::string dir_;
};
