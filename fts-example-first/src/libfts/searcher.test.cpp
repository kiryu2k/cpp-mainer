#include <libfts/searcher.hpp>

#include <gtest/gtest.h>

#include <cmath>

TEST(AccessorHealthCheck, TestHappyCase)
{
    const fts::ParserConfiguration config(3, 6, {"the"});
    fts::IndexBuilder index_builder;
    index_builder.add_document(100, "The Bourne Identity", config);
    index_builder.add_document(150, "The Bourne Supremacy", config);
    index_builder.add_document(1937, "The Bourne Ultimatum", config);
    char dir_template[] = "/tmp/index_XXXXXX";
    char* temp_dir = mkdtemp(dir_template);
    fts::TextIndexWriter text_index_writer;
    text_index_writer.write(temp_dir, index_builder.index());
    index_builder.add_document(2007, "The Bourne Legacy", config);
    text_index_writer.write(temp_dir, index_builder.index());
    fts::TextIndexAccessor accessor(temp_dir, config);
    EXPECT_EQ(accessor.total_docs(), 4);
    EXPECT_EQ(accessor.load_document(100), "The Bourne Identity");
    EXPECT_EQ(accessor.load_document(150), "The Bourne Supremacy");
    EXPECT_EQ(accessor.load_document(1937), "The Bourne Ultimatum");
    EXPECT_EQ(accessor.load_document(2007), "The Bourne Legacy");
    EXPECT_EQ(
            accessor.get_term_infos("bourn"),
            index_builder.index().entries_["bourn"]);
    std::filesystem::remove_all(temp_dir);
}

TEST(SearcherHealthCheck, TestHappyCase)
{
    const fts::ParserConfiguration config(5, 7, {});
    fts::IndexBuilder index_builder;
    index_builder.add_document(100, "Hello World", config);
    index_builder.add_document(101, "Byeee World", config);
    index_builder.add_document(102, "Hello Earth", config);
    char dir_template[] = "/tmp/index_XXXXXX";
    char* temp_dir = mkdtemp(dir_template);
    fts::TextIndexWriter text_index_writer;
    text_index_writer.write(temp_dir, index_builder.index());
    fts::TextIndexAccessor accessor(temp_dir, config);
    const std::string query = "hello world";
    const fts::Results exp_result = {
            {log(3.0 / 2) * 2, 100}, {log(3.0 / 2), 101}, {log(3.0 / 2), 102}};
    const std::vector<std::string> exp_text
            = {"Hello World", "Byeee World", "Hello Earth"};
    const auto searcher_config = fts::SearcherConfiguration(0);
    const auto result = fts::Searcher::search(query, accessor, searcher_config);
    for (std::size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i].score_, exp_result[i].score_);
        EXPECT_EQ(result[i].document_id_, exp_result[i].document_id_);
        EXPECT_EQ(accessor.load_document(result[i].document_id_), exp_text[i]);
    }
    std::filesystem::remove_all(temp_dir);
}

TEST(SearcherHealthCheck, CannotFindCase)
{
    const fts::ParserConfiguration config(5, 7, {});
    fts::IndexBuilder index_builder;
    index_builder.add_document(999, "learning cpp...", config);
    char dir_template[] = "/tmp/index_XXXXXX";
    char* temp_dir = mkdtemp(dir_template);
    fts::TextIndexWriter text_index_writer;
    text_index_writer.write(temp_dir, index_builder.index());
    fts::TextIndexAccessor accessor(temp_dir, config);
    const std::string query = "try hard";
    const auto searcher_config = fts::SearcherConfiguration(0);
    const auto result = fts::Searcher::search(query, accessor, searcher_config);
    EXPECT_EQ(result.size(), 0);
    std::filesystem::remove_all(temp_dir);
}