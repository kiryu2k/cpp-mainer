#pragma once

#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace csc::soundex {

class SoundexHashException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

std::string soundex_hash(std::string_view str);

} // namespace csc::soundex