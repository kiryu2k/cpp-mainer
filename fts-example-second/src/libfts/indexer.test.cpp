#include <libfts/indexer.hpp>

#include <gtest/gtest.h>

TEST(IndexerTest, AddSomeDocuments) {
    fts::ParserConfig config({"the"}, {3, 6}, 0);
    fts::IndexBuilder builder;
    builder.add_document(199903, "The Matrix", config);
    builder.add_document(200305, "The Matrix Reloaded", config);
    builder.add_document(200311, "The Matrix Revolutions", config);
    fts::ForwardIndex docs = {
        {199903, "The Matrix"},
        {200305, "The Matrix Reloaded"},
        {200311, "The Matrix Revolutions"}};
    EXPECT_EQ(builder.index().docs_, docs);
    fts::InvertedIndex entries = {
        {"mat", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"matr", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"matri", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"matrix", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"rel", {{200305, {1}}}},
        {"relo", {{200305, {1}}}},
        {"reloa", {{200305, {1}}}},
        {"reload", {{200305, {1}}}},
        {"rev", {{200311, {1}}}},
        {"revo", {{200311, {1}}}},
        {"revol", {{200311, {1}}}},
        {"revolu", {{200311, {1}}}}};
    std::vector<fts::Term> keys;
    for (const auto& [key, value] : entries) {
        keys.push_back(key);
    }
    for (const auto& [term, term_infos] : entries) {
        auto entry = entries.find(term);
        EXPECT_NE(entry, entries.end());
        for (std::size_t i = 0; i < term_infos.size(); ++i) {
            EXPECT_EQ(
                term_infos[i].document_id_, entry->second[i].document_id_);
            EXPECT_EQ(term_infos[i].positions_, entry->second[i].positions_);
        }
    }
}

TEST(IndexerTest, IndexAccessor) {
    fts::ParserConfig config({}, {3, 6}, 0);
    fts::IndexBuilder builder;
    builder.add_document(100, "Aenean sagittis risus massa", config);
    builder.add_document(200, "Proin ut risus tempor tincidunt nulla", config);
    builder.add_document(
        235, "tellus id mattis dolor sit amet     massa", config);
    char dir_name_template[] = "/tmp/index_XXXXXX";
    char* temporary_dir_name = mkdtemp(dir_name_template);
    fts::TextIndexWriter::write(temporary_dir_name, builder.index());
    fts::IndexAccessor accessor(temporary_dir_name);
    std::size_t docs_count = 3;
    EXPECT_EQ(accessor.total_docs(), docs_count);
    std::vector<fts::DocumentID> docs = {100, 235};
    EXPECT_EQ(accessor.get_documents_by_term("massa"), docs);
    EXPECT_EQ(
        accessor.load_document(200), "Proin ut risus tempor tincidunt nulla");
    std::vector<std::size_t> positions = {0};
    EXPECT_EQ(accessor.get_term_positions("tell", 235), positions);
}