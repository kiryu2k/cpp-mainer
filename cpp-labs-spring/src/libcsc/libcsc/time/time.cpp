#include <libcsc/time/time.hpp>

#include <iomanip>

namespace csc::time {

TimeSpan::TimeSpan(int sec) : sec_(sec) {}

Time::Time(int sec) : sec_(sec) {}

Time::Time(const TimeSpan &time) : sec_(time.sec()) {}

int TimeSpan::sec() const { return sec_; }

int Time::sec() const { return sec_; }

TimeSpan operator+(const TimeSpan &lhs, const TimeSpan &rhs) {
    return {lhs.sec() + rhs.sec()};
}

TimeSpan operator-(const TimeSpan &lhs, const TimeSpan &rhs) {
    return {lhs.sec() - rhs.sec()};
}

Time operator+(const Time &lhs, const TimeSpan &rhs) {
    return {lhs.sec() + rhs.sec()};
}

Time operator-(const Time &lhs, const TimeSpan &rhs) {
    return {lhs.sec() - rhs.sec()};
}

TimeSpan operator-(const Time &lhs, const Time &rhs) {
    return {lhs.sec() - rhs.sec()};
}

std::ostream &operator<<(std::ostream &os, const TimeSpan &time) {
    os << std::setfill('0') << std::setw(2) << time.sec_ / c_seconds_in_day
       << "d ";
    os << std::setfill('0') << std::setw(2)
       << (time.sec_ / c_seconds_in_hour) % c_hours_in_day << "h ";
    os << std::setfill('0') << std::setw(2)
       << (time.sec_ / c_seconds_in_minute) % c_minutes_in_hour << "m ";
    os << std::setfill('0') << std::setw(2) << time.sec_ % c_seconds_in_minute
       << "s";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Time &time) {
    os << std::setfill('0') << std::setw(2) << time.sec_ / c_seconds_in_day
       << "d ";
    os << std::setfill('0') << std::setw(2)
       << (time.sec_ / c_seconds_in_hour) % c_hours_in_day << "h ";
    os << std::setfill('0') << std::setw(2)
       << (time.sec_ / c_seconds_in_minute) % c_minutes_in_hour << "m ";
    os << std::setfill('0') << std::setw(2) << time.sec_ % c_seconds_in_minute
       << "s";
    return os;
}

namespace literals {
TimeSpan operator"" _d(unsigned long long sec) {
    return {static_cast<int>(sec * c_seconds_in_day)};
}

TimeSpan operator"" _h(unsigned long long sec) {
    return {static_cast<int>(sec * c_seconds_in_hour)};
}

TimeSpan operator"" _m(unsigned long long sec) {
    return {static_cast<int>(sec * c_seconds_in_minute)};
}

TimeSpan operator"" _s(unsigned long long sec) {
    return {static_cast<int>(sec)};
}
}; // namespace literals

} // namespace csc::time