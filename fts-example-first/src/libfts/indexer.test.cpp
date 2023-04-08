#include <libfts/indexer.hpp>

#include <gtest/gtest.h>

TEST(IndexerHealthCheck, TestHappyCase)
{
    const fts::ParserConfiguration config(3, 6, {"the"});
    fts::IndexBuilder index_builder;
    index_builder.add_document(100, "The Bourne Identity", config);
    index_builder.add_document(150, "The Bourne Supremacy", config);
    index_builder.add_document(1937, "The Bourne Ultimatum", config);
    index_builder.add_document(2007, "The Bourne Legacy", config);
    std::unordered_map<fts::DocID, fts::Doc> expected_docs
            = {{100, "The Bourne Identity"},
               {150, "The Bourne Supremacy"},
               {1937, "The Bourne Ultimatum"},
               {2007, "The Bourne Legacy"}};
    std::unordered_map<fts::Term, fts::TermInfos> expected_entries
            = {{"bou", {{100, {0}}, {150, {0}}, {1937, {0}}, {2007, {0}}}},
               {"bour", {{100, {0}}, {150, {0}}, {1937, {0}}, {2007, {0}}}},
               {"bourn", {{100, {0}}, {150, {0}}, {1937, {0}}, {2007, {0}}}},
               {"bourne", {{100, {0}}, {150, {0}}, {1937, {0}}, {2007, {0}}}},
               {"ide", {{100, {1}}}},
               {"iden", {{100, {1}}}},
               {"ident", {{100, {1}}}},
               {"identi", {{100, {1}}}},
               {"sup", {{150, {1}}}},
               {"supr", {{150, {1}}}},
               {"supre", {{150, {1}}}},
               {"suprem", {{150, {1}}}},
               {"ult", {{1937, {1}}}},
               {"ulti", {{1937, {1}}}},
               {"ultim", {{1937, {1}}}},
               {"ultima", {{1937, {1}}}},
               {"leg", {{2007, {1}}}},
               {"lega", {{2007, {1}}}},
               {"legac", {{2007, {1}}}},
               {"legacy", {{2007, {1}}}}};
    EXPECT_EQ(index_builder.index().docs_, expected_docs);
    EXPECT_EQ(index_builder.index().entries_, expected_entries);
}