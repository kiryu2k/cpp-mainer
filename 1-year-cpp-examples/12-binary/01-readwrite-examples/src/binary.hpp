#pragma once

#include <cstdint>
#include <cstring>

namespace binary {

template <typename T>
char* write(char* dest, const T& value) {
  std::memcpy(dest, &value, sizeof(value));
  return dest + sizeof(value);
}

template <typename StringType>
char* write_small_string(char* dest, const StringType& str) {
  char* cur = write<std::uint8_t>(dest, str.size());
  std::memcpy(cur, str.data(), str.size());
  return cur + str.size();
}

// TODO: generalize read functions
template <typename T>
const char* read(const char* buf, T& value) {
  std::memcpy(&value, buf, sizeof(value));
  return buf + sizeof(value);
}

template <typename T>
char* read(char* src, T& value) {
  std::memcpy(&value, src, sizeof(value));
  return src + sizeof(value);
}

template <typename StringType>
char* read_small_string(char* src, StringType& str) {
  std::uint8_t len;
  char* cur = read(src, len);
  str = StringType(cur, cur + len);
  return cur + len;
}

inline char* write_varint(char* buf, std::uint32_t value) {
  auto* cur = reinterpret_cast<std::uint8_t*>(buf);
  while (value >= 0x80) {
    const uint8_t byte = (value & 0x7f) | 0x80;
    *cur = byte;
    value >>= 7;
    ++cur;
  }
  *cur = value;
  ++cur;
  return reinterpret_cast<char*>(cur);
}

// TODO: implement read for const char*

inline char* read_varint(char* buf, std::uint32_t& value) {
  auto* cur = reinterpret_cast<std::uint8_t*>(buf);
  auto byte = *cur++;
  value = byte & 0x7f;
  std::size_t shift = 7;
  while (byte >= 0x80) {
    byte = *cur++;
    value += (byte & 0x7f) << shift;
    shift += 7;
  }
  return reinterpret_cast<char*>(cur);
}

}  // namespace binary

namespace binary_mut {

template <typename T>
void write(char*& dest, const T& value) {
  std::memcpy(dest, &value, sizeof(value));
  dest += sizeof(value);
}

template <typename StringType>
void write_small_string(char*& dest, const StringType& str) {
  write<std::uint8_t>(dest, str.size());
  std::memcpy(dest, str.data(), str.size());
  dest += str.size();
}

}  // namespace binary_mut
