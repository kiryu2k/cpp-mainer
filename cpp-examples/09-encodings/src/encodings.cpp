#include <fmt/format.h>
#include <fmt/ostream.h>
#include <unicode/coll.h>
#include <unicode/normalizer2.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <boost/locale.hpp>

#include <map>
#include <memory>
#include <string>

template <>
struct fmt::formatter<icu::UnicodeString> : fmt::ostream_formatter {};

void compare_std_string() {
  const std::string s1 = "résumé";
  const std::string s2 = "résumé";

  fmt::print("Compare str::string\n");
  fmt::print("{} == {}: {}\n", s1, s2, s1 == s2);
  fmt::print("{}.size() == {}\n", s1, s1.size());
  fmt::print("{}.size() == {}\n", s2, s2.size());
  fmt::print("\n");
}

void compare_unicode_string() {
  const icu::UnicodeString s1 = "résumé";
  const icu::UnicodeString s2 = "résumé";

  UErrorCode error = U_ZERO_ERROR;
  auto collator =
      std::unique_ptr<icu::Collator>(icu::Collator::createInstance(error));
  const bool eq = collator->equals(s1, s2);

  fmt::print("Compare icu::UnicodeString\n");
  fmt::print("{} == {}: {}\n", s1, s2, eq);
  fmt::print("{}.length() == {}\n", s1, s1.length());
  fmt::print("{}.length() == {}\n", s2, s2.length());
  fmt::print("\n");
}

void nfc() {
  const icu::UnicodeString s1 = "résumé";
  const icu::UnicodeString s2 = "résumé";

  UErrorCode error = U_ZERO_ERROR;
  auto norm = icu::Normalizer2::getNFCInstance(error);
  const auto norm_s1 = norm->normalize(s1, error);
  const auto norm_s2 = norm->normalize(s2, error);

  fmt::print("Normalization Form Canonical Composition\n");
  fmt::print("Before normalization:\n");
  fmt::print("{}.length() = {}\n", s1, s1.length());
  fmt::print("{}.length() = {}\n", s2, s2.length());
  fmt::print("After normalization:\n");
  fmt::print("{}.length() = {}\n", norm_s1, norm_s1.length());
  fmt::print("{}.length() = {}\n", norm_s2, norm_s2.length());
  fmt::print("\n");
}

void collation() {
  const icu::UnicodeString s1 = "résumé";
  const icu::UnicodeString s2 = "resume";

  UErrorCode error = U_ZERO_ERROR;
  auto collator =
      std::unique_ptr<icu::Collator>(icu::Collator::createInstance(error));
  collator->setStrength(icu::Collator::PRIMARY);

  fmt::print("Compare using primary collation strength\n");
  const bool eq = collator->equals(s1, s2);
  fmt::print("{} == {}: {}\n", s1, s2, eq);
  fmt::print("\n");
}

void word_list() {
  namespace bl = boost::locale;

  bl::generator gen;
  std::locale::global(gen(""));

  using PrimaryCmp = bl::comparator<char, bl::collator_base::primary>;
  std::map<std::string, std::size_t, PrimaryCmp> words;

  fmt::print("Generate frequency list\n");
  for (const std::string s : {"résumé", "resume"}) {
    fmt::print(" + {}\n", s);
    ++words[s];
  }

  for (const auto& [word, count] : words) {
    fmt::print("{}: {}\n", word, count);
  }
}

int main() {
  compare_std_string();
  compare_unicode_string();
  nfc();
  collation();
  word_list();
}
