#pragma once

#include <libfts/parser.hpp>

#include <filesystem>
#include <unordered_map>

namespace fts {

using DocID = std::size_t;
using Doc = std::string;
using Term = std::string;
using Position = std::vector<std::size_t>;
using TermInfos = std::unordered_map<DocID, Position>;

struct Index {
    std::unordered_map<DocID, Doc> docs_;
    std::unordered_map<Term, TermInfos> entries_;
};

struct IndexBuilder {
private:
    Index index_;

public:
    Index index();
    void add_document(
            DocID document_id,
            const Doc& text,
            const ParserConfiguration& config);
};

struct IndexException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct IndexWriter {
    virtual ~IndexWriter() = default;
    virtual void write(const std::filesystem::path& path, const Index& index)
            = 0;
};

struct TextIndexWriter : public IndexWriter {
    void write(const std::filesystem::path& path, const Index& index) override;
};

std::string string_to_hash(const std::string& string);

} // namespace fts