#pragma once

#include <libfts/parser.hpp>

#include <unordered_map>

namespace fts {

using DocumentID = std::size_t;
using Document = std::string;
using ForwardIndex = std::unordered_map<DocumentID, Document>;
using Positions = std::vector<std::size_t>;

struct TermInfo {
    DocumentID document_id_ = 0;
    Positions positions_;
};

using Term = std::string;
using TermInfos = std::vector<TermInfo>;
using InvertedIndex = std::unordered_map<Term, TermInfos>;

struct AccessorException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct Index {
    ForwardIndex docs_;
    InvertedIndex entries_;
};

struct IndexBuilder {
   private:
    Index index_;

   public:
    void add_document(
        std::size_t document_id,
        const std::string& text,
        const ParserConfig& config);
    Index index();
};

const std::size_t c_hash_size = 6;

struct TextIndexWriter {
    static void write(const std::string& path, const Index& index);
};

struct IndexAccessor {
   private:
    std::string index_path_;

   public:
    explicit IndexAccessor(std::string index_path);
    std::size_t total_docs() const;
    std::vector<DocumentID> get_documents_by_term(const Term& term) const;
    Document load_document(DocumentID document_id) const;
    Positions get_term_positions(const Term& term, DocumentID document_id)
        const;
};

}  // namespace fts