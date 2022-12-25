#include "binary.hpp"
#include "dump.hpp"

#include <fmt/printf.h>

#include <span>
#include <string>
#include <vector>

struct Person {
  std::string name_;
  int age_;
};

// Format:
// string = <len:u8><data:char[]>
// person = <name:string><age:u8>
//
// persons = <count:u16><data:person[]>

void dump_persons(std::span<char> buf, const std::vector<Person>& persons) {
  fmt::print("{} | {} ", dump(buf), persons.size());
  for (const auto& person : persons) {
    fmt::print("{} {} {} ", person.name_.size(), person.name_, person.age_);
  }
}

void dump_vector(
    std::span<char> buf,
    const std::vector<std::uint32_t>& values) {
  fmt::print("{} | {} ", dump(buf), values.size());
  for (const auto& values : values) {
    fmt::print("{} ", values);
  }
}

void test_struct_too_verbose() {
  const std::vector<Person> persons{{"James", 29}, {"Mary", 25}};

  char buf[16] = {};

  // Ugly
  char* cur = buf;
  const std::uint16_t size = persons.size();
  std::memcpy(cur, &size, sizeof(size));
  cur += sizeof(size);
  for (const auto& person : persons) {
    const std::uint8_t len = person.name_.size();
    std::memcpy(cur, &len, sizeof(len));
    cur += sizeof(len);
    std::memcpy(cur, person.name_.data(), len);
    cur += len;
    const std::uint8_t age = person.age_;
    std::memcpy(cur, &age, sizeof(age));
    cur += sizeof(age);
  }

  fmt::print("struct_too_verbose\n");
  dump_persons(buf, persons);
  fmt::print("\n\n");
}

void test_struct_using_functions() {
  const std::vector<Person> persons{{"James", 29}, {"Mary", 25}};

  char buf[16] = {};

  char* cur = buf;
  cur = binary::write<std::uint16_t>(cur, persons.size());
  for (const auto& person : persons) {
    cur = binary::write_small_string(cur, person.name_);
    cur = binary::write<std::uint8_t>(cur, person.age_);
  }

  fmt::print("struct_using_functions\n");
  dump_persons(buf, persons);
  fmt::print("\n\n");
}

void test_struct_using_functions_inout() {
  const std::vector<Person> persons{{"James", 29}, {"Mary", 25}};

  char buf[16] = {};

  // Ugly
  char* cur = buf;
  binary_mut::write<std::uint16_t>(cur, persons.size());
  for (const auto& person : persons) {
    binary_mut::write_small_string(cur, person.name_);
    binary_mut::write<std::uint8_t>(cur, person.age_);
  }

  fmt::print("struct_using_functions_inout\n");
  dump_persons(buf, persons);
  fmt::print("\n\n");
}

void test_read() {
  // clang-format off
  char buf[16] = {
      0x02, 0x00, 0x05, 0x4a, 0x61, 0x6d, 0x65, 0x73,
      0x1d, 0x04, 0x4d, 0x61, 0x72, 0x79, 0x19, 0x00
  };
  // clang-format on

  auto cur = buf;
  std::uint16_t count = 0;
  cur = binary::read(cur, count);
  std::vector<Person> persons;
  persons.reserve(count);
  for (std::uint16_t i = 0; i < count; ++i) {
    std::string name;
    std::uint8_t age = 0;
    cur = binary::read_small_string(cur, name);
    cur = binary::read(cur, age);
    persons.emplace_back(name, age);
  }

  fmt::print("read\n");
  dump_persons(buf, persons);
  fmt::print("\n\n");
}

void test_array() {
  std::vector<std::uint32_t> values = {3, 14, 15};

  char buf[16] = {};
  char* cur = buf;
  cur = binary::write_varint(cur, values.size());
  for (auto value : values) {
    cur = binary::write(cur, value);
  }
  fmt::print("array\n");
  dump_vector(buf, values);
  fmt::print("\n\n");
}

int main() {
  test_struct_too_verbose();
  test_struct_using_functions();
  test_struct_using_functions_inout();
  test_read();
  test_array();
}
