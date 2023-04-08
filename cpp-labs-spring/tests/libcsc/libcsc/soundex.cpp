#include <libcsc/soundex/soundex.hpp>

#include <gtest/gtest.h>

TEST(Soundex, ValidNames) {
    try {
        EXPECT_EQ(csc::soundex::soundex_hash("Aya"), "A000");
        EXPECT_EQ(csc::soundex::soundex_hash("Ayah"), "A000");
        EXPECT_EQ(csc::soundex::soundex_hash("Ava"), "A100");
        EXPECT_EQ(csc::soundex::soundex_hash("Abby"), "A100");
        EXPECT_EQ(csc::soundex::soundex_hash("Zerenity"), "Z653");
        EXPECT_EQ(csc::soundex::soundex_hash("Zorawar"), "Z660");
        EXPECT_EQ(csc::soundex::soundex_hash("Zarrar"), "Z660");
    } catch (const csc::soundex::SoundexHashException &ex) {
        FAIL();
    }
}

TEST(Soundex, InvalidNames) {
    EXPECT_THROW(
        { EXPECT_EQ(csc::soundex::soundex_hash("Aya5"), "FIVVVVE"); },
        csc::soundex::SoundexHashException);
    EXPECT_THROW(
        { EXPECT_EQ(csc::soundex::soundex_hash("@A!@##_"), "HEHEH"); },
        csc::soundex::SoundexHashException);
}