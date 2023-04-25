#include <libfts/searcher.hpp>

#include <algorithm>
#include <cmath>

namespace fts {

static SearchResults sort_result_by_score(
    const std::unordered_map<DocumentID, double>& search_result,
    const std::string& path) {
    SearchResults sorted_search_result;
    const IndexAccessor accessor(path);
    for (const auto& [document_id, score] : search_result) {
        sorted_search_result.push_back(
            {document_id, score, accessor.load_document(document_id)});
    }

    std::sort(
        sorted_search_result.begin(),
        sorted_search_result.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.score_ != rhs.score_
                ? lhs.score_ > rhs.score_
                : lhs.document_id_ < rhs.document_id_;
        });
    return sorted_search_result;
}

static SearchResults normalize_search_result(
    const std::unordered_map<DocumentID, double>& search_result,
    const std::string& path,
    double margin) {
    auto result = sort_result_by_score(search_result, path);

    /* remove books with small score */
    const auto cut_score = ((!result.empty()) ? result[0].score_ : 0) * margin;
    auto last = std::find_if(
        result.begin(), result.end(), [&cut_score](const auto& document) {
            return cut_score > document.score_;
        });
    result.erase(last, result.end());

    return result;
}

SearchResults search(
    const std::string& query,
    const ParserConfig& config,
    const std::string& path) {
    const IndexAccessor accessor(path);
    std::unordered_map<DocumentID, double> search_result;
    const auto parsed_query = parse(query, config);
    for (const auto& [term, position] : parsed_query) {
        try {
            const auto document_ids = accessor.get_documents_by_term(term);
            const auto document_freq = static_cast<double>(document_ids.size());
            for (const auto& document_id : document_ids) {
                const auto term_freq = static_cast<double>(
                    accessor.get_term_positions(term, document_id).size());
                search_result[document_id] += term_freq *
                    log(static_cast<double>(accessor.total_docs()) /
                        document_freq);
            }
        } catch (const AccessorException& e) {
            continue;
        }
    }
    return normalize_search_result(search_result, path, config.margin_);
}

}  // namespace fts