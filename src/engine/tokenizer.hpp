#ifndef ENGINE_TOKENIZER_HPP_
#define ENGINE_TOKENIZER_HPP_

#include <string>
#include <vector>

namespace Engine {

/**
 * TODO
 */
std::string removeCharacters(const std::string&       original,
                             const std::vector<char>& chars);

/**
 * TODO
 */
std::string removeWhitespace(const std::string& original);

/**
 * TODO
 */
std::vector<std::vector<char>> makeKGrams(const std::string& text, int k);

/**
 * TODO
 */
std::vector<unsigned int>
hashKGrams(const std::vector<std::vector<char>>& kGrams);

}  // namespace Engine

#endif  // ENGINE_TOKENIZER_HPP_
