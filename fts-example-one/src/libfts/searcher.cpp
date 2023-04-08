#include <libfts/searcher.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

namespace fts {

TextIndexAccessor::TextIndexAccessor(
        std::filesystem::path index_dir, ParserConfiguration config)
    : index_dir_{std::move(index_dir)}, config_{std::move(config)}
{
}

ParserConfiguration TextIndexAccessor::config() const
{
    return config_;
}

TermInfos TextIndexAccessor::get_term_infos(const Term& term) const
{
    const auto hash_term = string_to_hash(term);
    std::ifstream term_file(index_dir_ / "entries" / hash_term);
    if (!term_file.is_open()) {
        throw AccessorException("can't open file of " + term + " term");
    }
    Term word;
    term_file >> word;
    if (term != word) {
        throw AccessorException("term " + term + " doesn't exist");
    }
    TermInfos term_infos;
    std::size_t document_count = 0;
    term_file >> document_count;
    for (std::size_t i = 0; i < document_count; ++i) {
        std::size_t document_id = 0;
        term_file >> document_id;
        Position position;
        std::size_t position_count = 0;
        term_file >> position_count;
        for (std::size_t j = 0; j < position_count; ++j) {
            std::size_t position_number = 0;
            term_file >> position_number;
            position.push_back(position_number);
        }
        term_infos[document_id] = position;
    }
    return term_infos;
}

Doc TextIndexAccessor::load_document(DocID document_id) const
{
    std::ifstream document_file(
            index_dir_ / "docs" / std::to_string(document_id));
    Doc document;
    std::getline(document_file, document);
    return document;
}

std::size_t TextIndexAccessor::total_docs() const
{
    const auto index_dir_iter
            = std::filesystem::directory_iterator(index_dir_ / "docs");
    return std::count_if(
            begin(index_dir_iter), end(index_dir_iter), [](auto& file) {
                return file.is_regular_file();
            });
}

static void sort(Results& results)
{
    std::sort(
            results.begin(),
            results.end(),
            [](const auto& lhs, const auto& rhs) {
                return lhs.score_ != rhs.score_
                        ? lhs.score_ > rhs.score_
                        : lhs.document_id_ < rhs.document_id_;
            });
}

static double
find_max_score(const std::unordered_map<DocID, double>& search_result)
{
    const auto best_score = std::max_element(
            std::begin(search_result),
            std::end(search_result),
            [](const auto& lhs, const auto& rhs) {
                return lhs.second < rhs.second;
            });
    return (best_score == std::end(search_result)) ? 0 : best_score->second;
}

static void remove_docs_with_similar_names(
        Results& result, const IndexAccessor& index_accessor)
{
    std::unordered_set<Doc> document_set;
    const auto end_iter = std::remove_if(
            result.begin(),
            result.end(),
            [&document_set, &index_accessor](const auto& res) {
                const auto doc = index_accessor.load_document(res.document_id_);
                const auto is_found
                        = document_set.find(doc) != document_set.end();
                document_set.insert(doc);
                return is_found;
            });
    result.erase(end_iter, result.end());
}

Results Searcher::search(
        const std::string& query,
        const IndexAccessor& index_accessor,
        const SearcherConfiguration& searcher_config [[maybe_unused]])
{
    const auto parsed_query = parse_query(index_accessor.config(), query);
    std::unordered_map<DocID, double> result;
    for (const auto& word : parsed_query) {
        for (const auto& term : word.ngrams_) {
            TermInfos term_infos;
            try {
                term_infos = index_accessor.get_term_infos(term);
            } catch (const AccessorException& ex) {
                continue;
            }
            std::vector<DocID> documents;
            for (const auto& term_info : term_infos) {
                documents.push_back(term_info.first);
            }
            const auto document_frequency
                    = static_cast<double>(documents.size());
            for (const auto& document_id : documents) {
                const auto term_frequency
                        = static_cast<double>(term_infos[document_id].size());
                result[document_id] += term_frequency
                        * log(static_cast<double>(index_accessor.total_docs())
                              / document_frequency);
            }
        }
    }
    const auto highest_score = find_max_score(result);
    Results search_results;
    for (const auto& [document_id, score] : result) {
        if (highest_score * searcher_config.margin_ < score) {
            search_results.push_back({score, document_id});
        }
    }
    sort(search_results);
    remove_docs_with_similar_names(search_results, index_accessor);
    return search_results;
}

SearcherConfiguration::SearcherConfiguration(double margin) : margin_(margin)
{
}

SearcherConfiguration
SearcherConfiguration::load_configuration_file(const std::string& config_path)
{
    std::ifstream file(config_path);
    nlohmann::json parser_config = nlohmann::json::parse(file, nullptr, false);
    if (parser_config.is_discarded()) {
        throw ConfigurationException("incorrect format of config file");
    }
    const auto margin = parser_config["margin"].get<double>();
    return SearcherConfiguration(margin);
}

} // namespace fts