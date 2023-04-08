#include <libfts/parser.hpp>

#include <gtest/gtest.h>

TEST(HealthCheck, TestHappyCase)
{
    std::string query = "Everything was beautiful, and NOTHING hurt.";
    const fts::ParserConfiguration config(3, 6, {"and", "was"});
    const auto result = fts::parse_query(config, query);
    std::vector<fts::ParsedQuery> expected_result
            = {{{"eve", "ever", "every", "everyt"}, 0},
               {{"bea", "beau", "beaut", "beauti"}, 1},
               {{"not", "noth", "nothi", "nothin"}, 2},
               {{"hur", "hurt"}, 3}};
    for (std::size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i].ngrams_, expected_result[i].ngrams_);
        EXPECT_EQ(result[i].position_, expected_result[i].position_);
    }
}