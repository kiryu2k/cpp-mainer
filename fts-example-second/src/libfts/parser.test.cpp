#include <libfts/parser.hpp>

#include <gtest/gtest.h>

TEST(ParserTest, ParseTypicalText1) {
    std::string query = "Dr. Jekyll and Mr. Hyde";
    fts::ParserConfig config({"and", "dr", "mr"}, {3, 6}, 0);
    const auto result = fts::parse(query, config);
    fts::ParsedQuery expected_result = {
        {"jek", 0},
        {"jeky", 0},
        {"jekyl", 0},
        {"jekyll", 0},
        {"hyd", 1},
        {"hyde", 1}};
    for (std::size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i].text_position_, expected_result[i].text_position_);
        EXPECT_EQ(result[i].ngram_, expected_result[i].ngram_);
    }
}

TEST(ParserTest, ParseTypicalText2) {
    std::string query =
        "Lorem ipsum dolor siTt amet,      \t77 consectetur adipiscing elit!.";
    fts::ParserConfig config({"lorem", "ipsum", "adipiscing"}, {3, 5}, 0);
    const auto result = fts::parse(query, config);
    fts::ParsedQuery expected_result = {
        {"dol", 0},
        {"dolo", 0},
        {"dolor", 0},
        {"sit", 1},
        {"sitt", 1},
        {"ame", 2},
        {"amet", 2},
        {"con", 4},
        {"cons", 4},
        {"conse", 4},
        {"eli", 5},
        {"elit", 5}};
    for (std::size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i].text_position_, expected_result[i].text_position_);
        EXPECT_EQ(result[i].ngram_, expected_result[i].ngram_);
    }
}