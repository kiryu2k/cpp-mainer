#include <libcsc/soundex/soundex.hpp>

#include <algorithm>
#include <regex>
#include <vector>

namespace csc::soundex {

static char replace_char(char chr) {
    if ((chr == 'b') || (chr == 'f') || (chr == 'p') || (chr == 'v')) {
        return '1';
    }
    if ((chr == 'c') || (chr == 'g') || (chr == 'j') || (chr == 'k') ||
        (chr == 'q') || (chr == 's') || (chr == 'x') || (chr == 'z')) {
        return '2';
    }
    if ((chr == 'd') || (chr == 't')) {
        return '3';
    }
    if (chr == 'l') {
        return '4';
    }
    if ((chr == 'm') || (chr == 'n')) {
        return '5';
    }
    if (chr == 'r') {
        return '6';
    }
    return chr;
}

static bool has_only_letters(const std::string &str) {
    return std::regex_match(str, std::regex("^[A-Za-z]+$"));
}

static void string_to_lower(std::string &str) {
    std::transform(
        str.begin(), str.end(), str.begin(), [](const unsigned char chr) {
            return std::tolower(chr);
        });
}

static void remove_chars(std::string &str, const std::vector<char> &chars) {
    const auto remove_iter = std::remove_if(
        str.begin() + 1, str.end(), [&chars](const unsigned char chr) {
            return std::find(chars.begin(), chars.end(), chr) != chars.end();
        });
    str.erase(remove_iter, str.end());
}

static void replace_chars_to_numbers(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), replace_char);
}

static void remove_same_elem_sequence(std::string &str) {
    str.erase(std::unique(str.begin(), str.end()), str.end());
}

std::string soundex_hash(std::string_view str) {
    if (str.empty()) {
        throw SoundexHashException("trying to hash an empty string");
    }
    std::string hashed_str(str);
    if (!has_only_letters(hashed_str)) {
        throw SoundexHashException("trying to apply a phonetic algorithm to "
                                   "non-alphabetic characters");
    }
    string_to_lower(hashed_str);
    remove_chars(hashed_str, {'h', 'w'});
    replace_chars_to_numbers(hashed_str);
    remove_same_elem_sequence(hashed_str);
    remove_chars(hashed_str, {'a', 'e', 'i', 'o', 'u', 'y'});
    hashed_str.resize(4, '0');
    hashed_str[0] = static_cast<char>(std::toupper(str[0]));
    return hashed_str;
}

} // namespace csc::soundex