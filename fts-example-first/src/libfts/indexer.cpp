#include <libfts/indexer.hpp>

#include <fmt/format.h>
#include <picosha2/picosha2.h>

namespace fts {

std::string string_to_hash(const std::string& string)
{
    std::string hash_string;
    picosha2::hash256_hex_string(string, hash_string);
    return hash_string.substr(0, 6);
}

Index IndexBuilder::index()
{
    return index_;
}

void IndexBuilder::add_document(
        DocID document_id, const Doc& text, const ParserConfiguration& config)
{
    if (index_.docs_.find(document_id) != index_.docs_.end()) {
        throw IndexException(
                "document with " + std::to_string(document_id)
                + " ID already exists");
    }
    index_.docs_[document_id] = text;
    const auto parsed_text = parse_query(config, text);
    for (const auto& word : parsed_text) {
        for (const auto& ngram : word.ngrams_) {
            index_.entries_[ngram][document_id].push_back(word.position_);
        }
    }
}

void TextIndexWriter::write(
        const std::filesystem::path& path, const Index& index)
{
    std::filesystem::create_directories(path / "docs");
    std::filesystem::create_directories(path / "entries");
    for (const auto& [document_id, document] : index.docs_) {
        std::ofstream file(path / "docs" / std::to_string(document_id));
        file << document;
    }
    for (const auto& [term, entries] : index.entries_) {
        const auto hash_term = string_to_hash(term);
        std::ofstream file(path / "entries" / hash_term);
        auto string_entries = fmt::format("{} {} ", term, entries.size());
        for (const auto& [document_id, text_positions] : entries) {
            string_entries += fmt::format(
                    "{} {} ", document_id, text_positions.size());
            for (const auto& text_position : text_positions) {
                string_entries += fmt::format("{} ", text_position);
            }
        }
        file << string_entries;
    }
}

} // namespace fts