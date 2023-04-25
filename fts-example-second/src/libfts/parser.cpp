#include <libfts/parser.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>

namespace fts {

ParserConfig load_parser_config(const std::string& path) {
    std::ifstream file(path);
    const auto config = nlohmann::json::parse(file, nullptr, false);
    if (config.is_discarded()) {
        throw ConfigException("Incorrect configuration format!");
    }
    const auto min_ngram_length = config["min_ngram_length"].get<std::size_t>();
    const auto max_ngram_length = config["max_ngram_length"].get<std::size_t>();
    if (min_ngram_length >= max_ngram_length) {
        throw ConfigException(
            "Min value of the ngram length shouldn't be greater than max "
            "value!");
    }
    const auto stop_words =
        config["stop_words"].get<std::unordered_set<std::string>>();
    const auto margin = config["margin"].get<double>();
    return {stop_words, {min_ngram_length, max_ngram_length}, margin};
}

static void remove_punctuation(std::string& text) {
    text.erase(
        std::remove_if(
            text.begin(),
            text.end(),
            [](unsigned char symbol) { return std::ispunct(symbol); }),
        text.end());
}

static void convert_to_lowercase(std::string& text) {
    std::transform(
        text.begin(), text.end(), text.begin(), [](unsigned char symbol) {
            return std::tolower(symbol);
        });
}

static std::vector<std::string> split_string(std::string& text) {
    std::vector<std::string> words;
    auto first = std::find_if_not(text.begin(), text.end(), isspace);
    while (first != text.end()) {
        auto last = std::find_if(first, text.end(), isspace);
        std::string word = text.substr(first - text.begin(), last - first);
        words.push_back(word);
        first = std::find_if_not(last, text.end(), isspace);
    }
    return words;
}

static std::vector<std::string> remove_stop_words(
    const std::vector<std::string>& text,
    const std::unordered_set<std::string>& stop_words) {
    std::vector<std::string> text_without_stop_words;
    std::copy_if(
        text.begin(),
        text.end(),
        std::back_inserter(text_without_stop_words),
        [&stop_words](const std::string& word) {
            return stop_words.find(word) == stop_words.end();
        });
    return text_without_stop_words;
}

static ParsedQuery generate_ngrams(
    const std::vector<std::string>& words,
    const NgramLength& ngram_length) {
    ParsedQuery query;
    for (std::size_t i = 0; i < words.size(); ++i) {
        for (std::size_t j = ngram_length.min_; j <= ngram_length.max_; ++j) {
            if (words[i].length() >= j) {
                query.push_back({words[i].substr(0, j), i});
            }
        }
    }
    return query;
}

ParsedQuery parse(std::string text, const ParserConfig& config) {
    remove_punctuation(text);
    convert_to_lowercase(text);
    auto words = split_string(text);
    words = remove_stop_words(words, config.stop_words_);
    return generate_ngrams(words, config.ngram_length_);
}

}  // namespace fts