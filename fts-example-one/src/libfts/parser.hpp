#pragma once

#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace fts {

struct ParserConfiguration {
    std::size_t ngram_min_length_;
    std::size_t ngram_max_length_;
    std::unordered_set<std::string> stop_words_;
    ParserConfiguration(
            std::size_t ngram_min_length,
            std::size_t ngram_max_length,
            std::unordered_set<std::string> stop_words);
    static ParserConfiguration
    load_configuration_file(const std::string& config_path);
};

struct ConfigurationException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct ParsedQuery {
    std::vector<std::string> ngrams_;
    std::size_t position_ = 0;
};

std::vector<ParsedQuery>
parse_query(const ParserConfiguration& parser_config, std::string query);

} // namespace fts