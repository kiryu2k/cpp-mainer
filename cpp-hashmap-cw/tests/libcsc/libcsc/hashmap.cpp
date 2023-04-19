#include <libcsc/absolutepath.hpp>
#include <libcsc/hashmap/hashmap.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <unordered_map>

TEST(HashMap, HashMapTest) {
    const std::filesystem::path path(c_absolute_path);
    std::ifstream file(path / "words-5m.txt");
    std::istream_iterator<std::string> start(file), end;
    std::vector<std::string> words(start, end);
    csc::hashmap::HashMap<std::string, std::size_t> hashmap;
    std::unordered_map<std::string, std::size_t> exp;
    for (const auto &word : words) {
        ++hashmap[word];
        ++exp[word];
    }
    EXPECT_EQ(hashmap.size(), exp.size());
    for (auto &node : hashmap) {
        const auto iter = exp.find(node.key());
        if (iter == exp.end()) {
            FAIL();
        }
        EXPECT_EQ(node.key(), iter->first);
        EXPECT_EQ(node.value(), iter->second);
    }
}

TEST(HashMap, InsertElements) {
    csc::hashmap::HashMap<std::string, std::size_t> hashmap;
    EXPECT_EQ(hashmap.begin(), hashmap.end());
    hashmap["brackets"] = 88;
    EXPECT_EQ(hashmap.find("brackets")->value(), 88);
    const auto val = hashmap["constttttt"];
    EXPECT_EQ(hashmap.find("constttttt")->value(), val);
    hashmap.insert("brackets", 0);
    EXPECT_EQ(hashmap.find("brackets")->value(), 88);
}

TEST(HashMap, GetBucketSize) {
    csc::hashmap::HashMap<std::string, std::size_t> hashmap(1);
    EXPECT_EQ(hashmap.begin(), hashmap.end());
    hashmap["bucket"] = 8;
    hashmap["buckets"] = 88;
    hashmap["aaaaaaaaaaaaaaaaaaa"] = 91392;
    EXPECT_EQ(hashmap.bucket_count(), 1);
    EXPECT_EQ(hashmap.bucket_size(0), 3);
    hashmap.insert("aaaaaaaaaaaaaaaaaaa", 0);
    EXPECT_EQ(hashmap.bucket_size(0), 3);
}

TEST(HashMap, EraseElements) {
    csc::hashmap::HashMap<std::string, std::size_t> hashmap;
    EXPECT_EQ(hashmap.begin(), hashmap.end());
    EXPECT_EQ(hashmap.begin() == hashmap.begin(), true);
    EXPECT_EQ(hashmap.begin() == hashmap.end(), true);
    EXPECT_EQ(hashmap.size(), 0);
    hashmap["remove"] = 132213;
    EXPECT_EQ(hashmap.begin() == hashmap.begin(), true);
    EXPECT_EQ(hashmap.begin() == hashmap.end(), false);
    hashmap["delete"] = 93193121;
    hashmap["erase"] = 13213;
    hashmap["clear"] = 939123;
    hashmap["etc."] = 8192329321;
    EXPECT_EQ(hashmap.size(), 5);
    hashmap.erase("etc.");
    EXPECT_EQ(hashmap.size(), 4);
    /* trying to erase non-existent element */
    hashmap.erase("hahahahahahahaha");
    EXPECT_EQ(hashmap.size(), 4);
    hashmap.erase(hashmap.begin());
    EXPECT_EQ(hashmap.size(), 3);
    std::vector<std::string> keys;
    for (auto &node : hashmap) {
        keys.push_back(node.key());
    }
    EXPECT_EQ(keys.size(), 3);
    for (const auto &key : keys) {
        hashmap.erase(key);
    }
    EXPECT_EQ(hashmap.size(), 0);
}

TEST(HashMap, RuleOfFiveTest) {
    csc::hashmap::HashMap<std::string, std::size_t> hashmap;
    EXPECT_EQ(hashmap.begin(), hashmap.end());
    EXPECT_EQ(hashmap.size(), 0);
    hashmap["ummmmm"] = 132213;
    hashmap["hahahaahaha"] = 93193121;
    hashmap["mmmmmmmmmmmmm"] = 13213;
    hashmap["testtesttest"] = 939123;
    hashmap["yosh!!"] = 8192329321;
    EXPECT_EQ(hashmap.size(), 5);
    auto hashmap2(hashmap);
    EXPECT_EQ(hashmap.size(), hashmap2.size());
    EXPECT_EQ(hashmap.bucket_count(), hashmap2.bucket_count());
    for (auto &node : hashmap2) {
        auto iter = hashmap.find(node.key());
        if (iter == hashmap.end()) {
            FAIL();
        }
        EXPECT_EQ(node.key(), iter->key());
        EXPECT_EQ(node.value(), iter->value());
    }
    auto hashmap3(std::move(hashmap2));
    EXPECT_EQ(hashmap2.size(), 0);
    EXPECT_EQ(hashmap3.size(), 5);
    for (auto &node : hashmap3) {
        auto iter = hashmap.find(node.key());
        if (iter == hashmap.end()) {
            FAIL();
        }
        EXPECT_EQ(node.key(), iter->key());
        EXPECT_EQ(node.value(), iter->value());
    }
    auto hashmap4 = hashmap;
    EXPECT_EQ(hashmap4.size(), 5);
    for (auto &node : hashmap4) {
        auto iter = hashmap.find(node.key());
        if (iter == hashmap.end()) {
            FAIL();
        }
        EXPECT_EQ(node.key(), iter->key());
        EXPECT_EQ(node.value(), iter->value());
    }
    hashmap = hashmap;
    EXPECT_EQ(hashmap.size(), 5);
    hashmap2 = std::move(hashmap4);
    EXPECT_EQ(hashmap4.size(), 0);
    EXPECT_EQ(hashmap.size(), hashmap2.size());
    EXPECT_EQ(hashmap.bucket_count(), hashmap2.bucket_count());
    for (auto &node : hashmap2) {
        auto iter = hashmap.find(node.key());
        if (iter == hashmap.end()) {
            FAIL();
        }
        EXPECT_EQ(node.key(), iter->key());
        EXPECT_EQ(node.value(), iter->value());
    }
    hashmap2 = std::move(hashmap2);
}

TEST(HashMap, StlAlgorithms) {
    csc::hashmap::HashMap<std::string, std::string> hashmap;
    hashmap["fox"] = "bop";
    hashmap["strong"] = "zero gravity";
    hashmap["don't"] = "wanna talk";
    hashmap["haunted"] = "castle";
    hashmap["masquerade"] = "memememememe";
    auto exp = std::max_element(
        hashmap.begin(), hashmap.end(), [](auto &lhs, auto &rhs) {
            return lhs.key() < rhs.key();
        });
    EXPECT_EQ(exp->key(), "strong");
    exp = std::find_if(hashmap.begin(), hashmap.end(), [](auto &iter) {
        return iter.value() == "castle";
    });
    EXPECT_EQ(exp->key(), "haunted");
    auto node = *hashmap.find("masquerade");
    exp = std::find(hashmap.begin(), hashmap.end(), node);
    EXPECT_EQ(exp->key(), node.key());
    EXPECT_EQ(exp->value(), node.value());
}