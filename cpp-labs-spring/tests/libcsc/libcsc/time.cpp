#include <libcsc/time/time.hpp>

#include <gtest/gtest.h>

#include <sstream>

using namespace csc::time::literals;

TEST(Time, TimeOperations) {
    csc::time::Time time1(18_d);
    csc::time::Time time2(22_h);
    csc::time::TimeSpan time_span1 = 55_m;
    csc::time::TimeSpan time_span2 = 16_s;
    EXPECT_EQ(time1 - time2, csc::time::TimeSpan(18_d - 22_h));
    EXPECT_EQ(time1 + time_span1, csc::time::Time(18_d + 55_m));
    EXPECT_EQ(time_span2 + time_span1, csc::time::TimeSpan(55_m + 16_s));
    EXPECT_EQ(time_span1 - time_span2, csc::time::TimeSpan(55_m - 16_s));
    time1 = time1 - time_span2;
    EXPECT_EQ(time1, csc::time::Time(18_d - 16_s));
    time2 = time2 + time_span1;
    EXPECT_EQ(time2, csc::time::Time(22_h + 55_m));
}

TEST(Time, TimeOutputOperator) {
    std::ostringstream os;
    os << csc::time::Time(1_d + 23_h + 10_m + 11_s) << "\n";
    os << csc::time::TimeSpan(37_s) << "\n";
    EXPECT_EQ(os.str(), "01d 23h 10m 11s\n00d 00h 00m 37s\n");
}

TEST(Time, TimeComparison) {
    EXPECT_TRUE(csc::time::Time(24_h) == csc::time::Time(1_d));
    EXPECT_TRUE(csc::time::Time(23_h) != csc::time::Time(1_d));
    EXPECT_TRUE(csc::time::TimeSpan(21_m) >= csc::time::TimeSpan(1_s));
}