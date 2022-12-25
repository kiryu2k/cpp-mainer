#pragma once

#include <fmt/format.h>

#include <span>
#include <string>

inline std::string dump(std::span<char> data) {
  return fmt::format("{:02x}", fmt::join(data, " "));
}
