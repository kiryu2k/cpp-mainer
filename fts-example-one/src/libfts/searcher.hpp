#pragma once

#include <libfts/indexer.hpp>

namespace fts {

struct AccessorException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct IndexAccessor {
public:
    virtual ~IndexAccessor() = default;
    virtual ParserConfiguration config() const = 0;
    virtual TermInfos get_term_infos(const Term& term) const = 0;
    virtual Doc load_document(DocID document_id) const = 0;
    virtual std::size_t total_docs() const = 0;
};

struct TextIndexAccessor : public IndexAccessor {
private:
    std::filesystem::path index_dir_;
    ParserConfiguration config_;

public:
    TextIndexAccessor(
            std::filesystem::path index_dir, ParserConfiguration config);
    ParserConfiguration config() const override;
    TermInfos get_term_infos(const Term& term) const override;
    Doc load_document(DocID document_id) const override;
    std::size_t total_docs() const override;
};

struct SearcherConfiguration {
    /* possible improving of search engine configuration in future... */
    double margin_;
    explicit SearcherConfiguration(double margin);
    static SearcherConfiguration
    load_configuration_file(const std::string& config_path);
};

struct Result {
    double score_;
    DocID document_id_;
};

using Results = std::vector<Result>;

struct Searcher {
public:
    static Results
    search(const std::string& query,
           const IndexAccessor& index_accessor,
           const SearcherConfiguration& searcher_config);
};

} // namespace fts