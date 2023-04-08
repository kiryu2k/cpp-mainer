#include <libfts/parser.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>

namespace fts {

ParserConfiguration
ParserConfiguration::load_configuration_file(const std::string& config_path)
{
    std::ifstream file(config_path);
    nlohmann::json parser_config = nlohmann::json::parse(file, nullptr, false);
    if (parser_config.is_discarded()) {
        throw ConfigurationException("incorrect format of config file");
    }
    const auto ngram_min_length
            = parser_config["ngram_min_length"].get<std::size_t>();
    const auto ngram_max_length
            = parser_config["ngram_max_length"].get<std::size_t>();
    const auto stop_words = parser_config["stop_words"]
                                    .get<std::unordered_set<std::string>>();
    return {ngram_min_length, ngram_max_length, stop_words};
}

ParserConfiguration::ParserConfiguration(
        std::size_t ngram_min_length,
        std::size_t ngram_max_length,
        std::unordered_set<std::string> stop_words)
    : ngram_min_length_{ngram_min_length},
      ngram_max_length_{ngram_max_length},
      stop_words_{std::move(stop_words)}
{
    if (ngram_min_length_ >= ngram_max_length_) {
        throw ConfigurationException(
                "min ngram length must be less than max ngram length");
    }
}

static void delete_punctuation(std::string& string)
{
    string.erase(
            std::remove_if(
                    string.begin(),
                    string.end(),
                    [](unsigned char sym) { return std::ispunct(sym); }),
            string.end());
}

static void string_to_lower(std::string& string)
{
    std::transform(
            string.begin(),
            string.end(),
            string.begin(),
            [](unsigned char sym) { return std::tolower(sym); });
}

static std::vector<std::string> split_string(std::string& string)
{
    std::vector<std::string> splitted_string;
    auto left_border = std::find_if_not(string.begin(), string.end(), isspace);
    while (left_border != string.end()) {
        auto right_border = std::find_if(left_border, string.end(), isspace);
        std::string word = string.substr(
                left_border - string.begin(), right_border - left_border);
        splitted_string.push_back(word);
        left_border = std::find_if_not(right_border, string.end(), isspace);
    }
    return splitted_string;
}

static std::vector<std::string> remove_stop_words(
        const std::unordered_set<std::string>& stop_words,
        const std::vector<std::string>& text)
{
    std::vector<std::string> result;
    std::copy_if(
            text.begin(),
            text.end(),
            std::back_inserter(result),
            [&stop_words](const std::string& word) {
                return stop_words.find(word) == stop_words.end();
            });
    return result;
}

static std::vector<ParsedQuery> transform_word_to_ngrams(
        const std::vector<std::string>& words,
        const ParserConfiguration& parser_config)
{
    std::vector<ParsedQuery> result;
    for (std::size_t i = 0; i < words.size(); ++i) {
        ParsedQuery word;
        for (std::size_t j = parser_config.ngram_min_length_;
             j <= parser_config.ngram_max_length_;
             ++j) {
            if (words[i].length() < j) {
                break;
            }
            word.ngrams_.push_back(words[i].substr(0, j));
        }
        if (!word.ngrams_.empty()) {
            word.position_ = i;
            result.push_back(word);
        }
    }
    return result;
}

std::vector<ParsedQuery>
parse_query(const ParserConfiguration& parser_config, std::string query)
{
    delete_punctuation(query);
    string_to_lower(query);
    auto words
            = remove_stop_words(parser_config.stop_words_, split_string(query));
    return transform_word_to_ngrams(words, parser_config);
}

} // namespace fts