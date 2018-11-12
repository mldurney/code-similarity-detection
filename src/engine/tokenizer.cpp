#include <algorithm>
#include <exception>

#include "tokenizer.hpp"

using namespace Engine;

std::string Engine::removeCharacters(const std::string&       original,
                                     const std::vector<char>& chars) {
    std::string modified;
    for (auto c : original) {
        if (std::find(chars.begin(), chars.end(), c) != chars.end()) {
            modified += c;
        }
    }

    return modified;
}

std::string Engine::removeWhitespace(const std::string& original) {
    std::vector<char> whitespaceTokens = {' ', '\t', '\n', '\v', '\f', '\r'};
    return removeCharacters(original, whitespaceTokens);
}

std::vector<std::vector<char>> Engine::makeKGrams(const std::string& text,
                                                  int                k) {
    if (k < 1) {
        std::string message
            = "makeKGrams: received k = " + std::to_string(k) + "; need k > 1";
        throw std::invalid_argument(message);
    }

    // Pre-reserve space for all k-grams
    std::vector<std::vector<char>> kGrams(text.size() - k + 1,
                                          std::vector<char>(k));

    // Copy characters from string into each k-gram
    for (unsigned int i = 0; i < text.size(); i++) {
        for (unsigned int j = 0; j < k; j++) {
            kGrams[i][j] = text[i + j];
        }
    }

    return kGrams;
}

std::vector<unsigned int>
Engine::hashKGrams(const std::vector<std::vector<char>>& kGrams) {
    std::vector<unsigned int> hashes(kGrams.size());

    // TODO

    return hashes;
}
