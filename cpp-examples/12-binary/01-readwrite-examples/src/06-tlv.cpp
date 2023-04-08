#include "binary.hpp"
#include "dump.hpp"

#include <memory>
#include <vector>

struct Point {
  std::int32_t x_ = 0;
  std::int32_t y_ = 0;
};

struct Shape {
  virtual ~Shape() = default;
};

using ShapePtr = std::unique_ptr<Shape>;

enum class Tag : std::uint8_t {
  Triangle = 1,
  Circle = 2
};

struct Triangle : public Shape {
  Triangle() = default;
  Triangle(Point a, Point b, Point c) : a_(a), b_(b), c_(c) {
  }

  Point a_, b_, c_;
};

struct Circle : public Shape {
  Circle() = default;
  Circle(Point center, double radius) : center_(center), radius_(radius) {
  }

  Point center_;
  double radius_;
};

ShapePtr read_triangle(char* buf) {
  auto triangle = std::make_unique<Triangle>();
  buf = binary::read(buf, triangle->a_);
  buf = binary::read(buf, triangle->b_);
  buf = binary::read(buf, triangle->c_);
  return triangle;
}

ShapePtr read_circle(char* buf) {
  auto circle = std::make_unique<Circle>();
  buf = binary::read(buf, circle->center_);
  buf = binary::read(buf, circle->radius_);
  return circle;
}

// Format:
// <count:u32>
// [{<tag:u8> <length:u8> <value:?>} ...]
void test_tlv() {
  // clang-format off
  char buf[] = {
    0x03, 0x00, 0x00, 0x00, // count
    0x01, // triangle tag
    0x18, // length
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // a
    0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // b
    0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // c
    0x03, // unknown tag
    0x20, // length
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ?
    0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ?
    0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, // ?
    0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, // ?
    0x02, // circle tag
    0x18, // length
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // center
    0x1f, -0x7b, -0x15, 0x51, -0x48, 0x1e, 0x09, 0x40, // radius
  };
  // clang-format on

  char* cur = buf;
  std::uint32_t count = 0;
  cur = binary::read(cur, count);

  std::vector<ShapePtr> shapes;
  for (std::uint32_t i = 0; i < count; ++i) {
    Tag tag;
    std::uint8_t length;
    cur = binary::read(cur, tag);
    cur = binary::read(cur, length);
    switch (tag) {
      case Tag::Circle:
        shapes.push_back(read_circle(cur));
        break;
      case Tag::Triangle:
        shapes.push_back(read_triangle(cur));
        break;
    }
    cur += length;
  }

  for (const auto& shape : shapes) {
    if (auto circle = dynamic_cast<const Circle*>(shape.get())) {
      fmt::print(
          "Circle({} {}, {})\n",
          circle->center_.x_,
          circle->center_.y_,
          circle->radius_);
    } else if (auto triangle = dynamic_cast<const Triangle*>(shape.get())) {
      fmt::print(
          "Triangle({} {}, {} {}, {} {})\n",
          triangle->a_.x_,
          triangle->a_.y_,
          triangle->b_.x_,
          triangle->b_.y_,
          triangle->c_.x_,
          triangle->c_.y_);
    }
  }
  fmt::print("\n");
}

char* write_circle(char* buf, const Circle& circle) {
  char* cur = buf;
  cur = binary::write(cur, Tag::Circle);
  cur = binary::write(cur, circle.center_);
  cur = binary::write(cur, circle.radius_);
  return cur;
}

char* write_triangle(char* buf, const Triangle& triangle) {
  char* cur = buf;
  cur = binary::write(buf, Tag::Triangle);
  cur = binary::write(cur, triangle.a_);
  cur = binary::write(cur, triangle.b_);
  cur = binary::write(cur, triangle.c_);
  return cur;
}

// Format:
// [<offset:u32> ...]
// [{<tag:u8?> <object:?>} ...]
void test_tlv_constant_indexing() {
  std::vector<ShapePtr> shapes;

  shapes.push_back(
      std::make_unique<Triangle>(Point{1, 1}, Point{2, 2}, Point{3, 3}));
  shapes.push_back(std::make_unique<Circle>(Point{1, 2}, 3.1415));
  shapes.push_back(
      std::make_unique<Triangle>(Point{1, 1}, Point{2, 2}, Point{3, 3}));

  char buf[1024] = {};

  auto* offsets = reinterpret_cast<std::uint32_t*>(buf);
  // reserve space for offsets
  char* cur = buf + sizeof(std::uint32_t) * shapes.size();

  // serialize
  for (std::size_t i = 0; i < shapes.size(); ++i) {
    const auto& shape = shapes[i];
    offsets[i] = cur - buf;
    if (auto circle = dynamic_cast<const Circle*>(shape.get())) {
      cur = write_circle(cur, *circle);
    } else if (auto triangle = dynamic_cast<const Triangle*>(shape.get())) {
      cur = write_triangle(cur, *triangle);
    }
  }

  // access i-th object
  fmt::print("i-th object\n");
  std::uint32_t circle_offset = offsets[1];
  char* circle_data = buf + circle_offset + sizeof(Tag);
  auto shape_ptr = read_circle(circle_data);
  if (auto* circle = dynamic_cast<Circle*>(shape_ptr.get())) {
    fmt::print(
        "Circle({} {}, {})\n",
        circle->center_.x_,
        circle->center_.y_,
        circle->radius_);
  }
}

int main() {
  test_tlv();
  test_tlv_constant_indexing();
}
