#pragma once
#include "InvertedIndex.h"
#include "IndexIO.h"
#include <string>

class IndexReader {
public:
    explicit IndexReader(const std::string& dir);
    InvertedIndex read(int& corpusSize);

private:
    std::string dir_;
};
