#include "Utils.h"

std::string remove_duplicates(const std::string& input) {
    std::istringstream iss(input);
    std::string word;
    std::set<std::string> words_set;
    std::vector<std::string> ordered_result;

    while (iss >> word) {
        if (words_set.insert(word).second) {
            ordered_result.push_back(word);
        }
    }

    std::ostringstream oss;
    for (size_t i = 0; i < ordered_result.size(); ++i) {
        if (i > 0) {
            oss << " ";
        }
        oss << ordered_result[i];
    }

    return oss.str();
}
