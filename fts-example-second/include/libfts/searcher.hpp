#pragma once

#include <libfts/indexer.hpp>

namespace fts {

struct SearchResult {
    DocumentID document_id_ = 0;
    double score_ = 0.;
    Document document_;
};

using SearchResults = std::vector<SearchResult>;

SearchResults search(
    const std::string& query,
    const ParserConfig& config,
    const std::string& path);

}  // namespace fts