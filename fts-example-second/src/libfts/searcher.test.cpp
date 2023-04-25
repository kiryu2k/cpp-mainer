#include <libfts/searcher.hpp>

#include <gtest/gtest.h>

#include <cmath>
#include <sys/types.h>

TEST(SearcherTest, CalculateScore) {
    fts::ParserConfig config({}, {5, 6}, 0);
    fts::IndexBuilder builder;
    builder.add_document(100, "Hello World", config);
    builder.add_document(101, "Byeee World", config);
    builder.add_document(102, "Hello Earth", config);
    char dir_name_template[] = "/tmp/index_XXXXXX";
    char* temporary_dir_name = mkdtemp(dir_name_template);
    fts::TextIndexWriter::write(temporary_dir_name, builder.index());
    fts::IndexAccessor accessor(temporary_dir_name);
    std::string query = "hello world";
    fts::SearchResults expected_search_result = {
        {100, log(3.0 / 2) * 2, "Hello World"},
        {101, log(3.0 / 2), "Byeee World"},
        {102, log(3.0 / 2), "Hello Earth"}};
    const auto search_result = fts::search(query, config, temporary_dir_name);
    for (std::size_t i = 0; i < search_result.size(); ++i) {
        EXPECT_EQ(
            search_result[i].document_id_,
            expected_search_result[i].document_id_);
        EXPECT_EQ(
            search_result[i].document_, expected_search_result[i].document_);
        EXPECT_EQ(search_result[i].score_, expected_search_result[i].score_);
    }
}