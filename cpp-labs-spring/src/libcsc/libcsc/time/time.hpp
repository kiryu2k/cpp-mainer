#pragma once

#include <compare>
#include <cstdint>
#include <ostream>

namespace csc::time {

const std::size_t c_hours_in_day = 24;
const std::size_t c_minutes_in_hour = 60;
const std::size_t c_seconds_in_minute = 60;
const std::size_t c_seconds_in_hour = c_seconds_in_minute * c_minutes_in_hour;
const std::size_t c_seconds_in_day = c_seconds_in_hour * c_hours_in_day;

class TimeSpan {
private:
    int sec_;

public:
    TimeSpan(int sec = 0);
    int sec() const;
    friend std::ostream &operator<<(std::ostream &os, const TimeSpan &time);
    auto operator<=>(const TimeSpan &rhs) const = default;
};

class Time {
private:
    int sec_;

public:
    Time(int sec = 0);
    explicit Time(const TimeSpan &time);
    int sec() const;
    friend std::ostream &operator<<(std::ostream &os, const Time &time);
    auto operator<=>(const Time &rhs) const = default;
};

TimeSpan operator+(const TimeSpan &lhs, const TimeSpan &rhs);
TimeSpan operator-(const TimeSpan &lhs, const TimeSpan &rhs);

Time operator+(const Time &lhs, const TimeSpan &rhs);
Time operator-(const Time &lhs, const TimeSpan &rhs);

TimeSpan operator-(const Time &lhs, const Time &rhs);

namespace literals {
TimeSpan operator"" _d(unsigned long long sec);

TimeSpan operator"" _h(unsigned long long sec);

TimeSpan operator"" _m(unsigned long long sec);

TimeSpan operator"" _s(unsigned long long sec);
}; // namespace literals

} // namespace csc::time