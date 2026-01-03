#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class Tokenizer{
    public:
    Tokenizer();
    std::vector<std::string> tokenize(const std::string& text) const;

    private:
    std::unordered_set<std::string> stopWords_;
};
