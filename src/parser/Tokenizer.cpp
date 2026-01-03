#include "Tokenizer.h"
#include <cctype>

Tokenizer::Tokenizer() {
    // A standard list of English stop words
    stopWords_ = {
        "a", "an", "and", "are", "as", "at", "be", "but", "by",
        "for", "if", "in", "into", "is", "it", "no", "not", "of",
        "on", "or", "such", "that", "the", "their", "then", "there",
        "these", "they", "this", "to", "was", "will", "with"
    };
}

std::vector<std::string> Tokenizer::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::string current;

    for (char c : text) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            current += static_cast<char>(std::tolower(c));
        } else {
            if (!current.empty()) {
                // CHANGE IS HERE: Check if it's a stop word
                if (stopWords_.find(current) == stopWords_.end()) {
                    tokens.push_back(current);
                }
                current.clear();
            }
        }
    }

    // Handle the last token
    if (!current.empty()) {
        if (stopWords_.find(current) == stopWords_.end()) {
            tokens.push_back(current);
        }
    }

    return tokens;
}