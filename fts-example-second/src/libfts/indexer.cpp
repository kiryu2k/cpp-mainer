#include <libfts/indexer.hpp>

#include <filesystem>
#include <picosha2/picosha2.h>

namespace fts {

static std::string generate_hash_term(const std::string& term) {
    std::string hash_term;
    picosha2::hash256_hex_string(term, hash_term);
    return hash_term.substr(0, c_hash_size);
}

void IndexBuilder::add_document(
    std::size_t document_id,
    const std::string& text,
    const ParserConfig& config) {
    if (index_.docs_.find(document_id) != index_.docs_.end()) {
        return;
    }
    index_.docs_[document_id] = text;
    const auto parsed_text = parse(text, config);
    std::unordered_map<Term, Positions> entries;
    for (const auto& [term, position] : parsed_text) {
        entries[term].push_back(position);
    }
    for (const auto& [term, positions] : entries) {
        index_.entries_[term].push_back({document_id, positions});
    }
}

Index IndexBuilder::index() {
    return index_;
}

static void write_docs(const ForwardIndex& docs, const std::string& path) {
    std::filesystem::create_directories(path + "/docs");
    for (const auto& doc : docs) {
        std::ofstream file(path + "/docs/" + std::to_string(doc.first));
        file << doc.second;
    }
}

static std::string to_string_entries(
    const Term& term,
    const TermInfos& term_infos) {
    std::string result = term + " " + std::to_string(term_infos.size()) + " ";
    for (const auto& term_info : term_infos) {
        result += std::to_string(term_info.document_id_) + " " +
            std::to_string(term_info.positions_.size()) + " ";
        for (const auto& position : term_info.positions_) {
            result += std::to_string(position) + " ";
        }
    }
    return result;
}

static void write_entries(
    const InvertedIndex& entries,
    const std::string& path) {
    std::filesystem::create_directories(path + "/entries");
    for (const auto& [term, term_infos] : entries) {
        std::string hash_term = generate_hash_term(term);
        std::ofstream file(path + "/entries/" + hash_term);
        file << to_string_entries(term, term_infos);
    }
}

void TextIndexWriter::write(const std::string& path, const Index& index) {
    write_docs(index.docs_, path);
    write_entries(index.entries_, path);
}

IndexAccessor::IndexAccessor(std::string index_path)
    : index_path_{std::move(index_path)} {
}

std::size_t IndexAccessor::total_docs() const {
    const auto index_dir_iter =
        std::filesystem::directory_iterator(index_path_ + "/docs");
    return std::count_if(
        begin(index_dir_iter), end(index_dir_iter), [](auto& entry) {
            return entry.is_regular_file();
        });
}

Document IndexAccessor::load_document(DocumentID document_id) const {
    std::ifstream file(index_path_ + "/docs/" + std::to_string(document_id));
    Document document;
    std::getline(file, document);
    return document;
}

static void parse_entry(InvertedIndex& entries, const std::string& path) {
    std::ifstream file(path);
    Term term;
    file >> term;
    std::size_t document_count = 0;
    file >> document_count;
    for (std::size_t i = 0; i < document_count; ++i) {
        std::size_t document_id = 0;
        file >> document_id;
        Positions position;
        std::size_t positions_count = 0;
        file >> positions_count;
        for (std::size_t j = 0; j < positions_count; ++j) {
            std::size_t position_number = 0;
            file >> position_number;
            position.push_back(position_number);
        }
        entries[term].push_back({document_id, position});
    }
}

std::vector<DocumentID> IndexAccessor::get_documents_by_term(
    const Term& term) const {
    std::string hash_term = generate_hash_term(term);
    InvertedIndex entries;
    parse_entry(entries, index_path_ + "/entries/" + hash_term);
    if (entries.find(term) == entries.end()) {
        throw AccessorException("Failed to get list of documents by term!");
    }
    std::vector<DocumentID> result;
    for (const auto& [document_id, position] : entries[term]) {
        result.push_back(document_id);
    }
    return result;
}

Positions IndexAccessor::get_term_positions(
    const Term& term,
    DocumentID document_id) const {
    std::string hash_term = generate_hash_term(term);
    InvertedIndex entries;
    parse_entry(entries, index_path_ + "/entries/" + hash_term);
    if (entries.find(term) == entries.end()) {
        throw AccessorException("Failed to get list of term positions!");
    }
    auto term_infos = entries.find(term)->second;
    for (const auto& term_info : term_infos) {
        if (term_info.document_id_ == document_id) {
            return term_info.positions_;
        }
    }
    throw AccessorException("Failed to get list of term positions!");
    return {};
}

}  // namespace fts