#pragma once

#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace fts {

struct ConfigException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct NgramLength {
    std::size_t min_ = 0;
    std::size_t max_ = 0;
};

struct ParserConfig {
    std::unordered_set<std::string> stop_words_;
    NgramLength ngram_length_;
    ParserConfig() = default;
    ParserConfig(
        std::unordered_set<std::string> stop_words,
        const NgramLength& ngram_length,
        double margin)
        : stop_words_{std::move(stop_words)},
          ngram_length_{ngram_length},
          margin_{margin} {
    }
    double margin_ = 0;
};

struct NgramInfo {
    std::string ngram_;
    std::size_t text_position_ = 0;
};

using ParsedQuery = std::vector<NgramInfo>;

ParserConfig load_parser_config(const std::string& path);

ParsedQuery parse(std::string text, const ParserConfig& config);

}  // namespace fts