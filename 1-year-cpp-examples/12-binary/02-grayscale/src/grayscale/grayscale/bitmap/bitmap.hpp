#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace bitmap {

#pragma pack(push, 2)
struct Header {
  std::uint16_t type_;
  std::uint32_t size_;
  std::uint16_t reserved1_;
  std::uint16_t reserved2_;
  std::uint32_t pixel_offset_;
};

struct DibHeader {
  std::uint32_t header_size_;
  std::uint32_t width_;
  std::uint32_t height_;
};
#pragma pack(pop)

struct Pixel {
  std::uint8_t red_;
  std::uint8_t green_;
  std::uint8_t blue_;
};

struct BitmapImage {
  BitmapImage(char* data, std::size_t size)
      : data_(data),
        size_(size),
        header_(reinterpret_cast<Header*>(data_)),
        dib_header_(reinterpret_cast<DibHeader*>(data_ + sizeof(Header))) {
  }

  const Pixel* pixels_begin() const {
    return reinterpret_cast<const Pixel*>(data_ + header_->pixel_offset_);
  }

  const Pixel* pixels_end() const {
    const auto pixels_count = dib_header_->width_ * dib_header_->height_;
    return pixels_begin() + pixels_count;
  }

  char* data_;
  std::size_t size_;
  Header* header_;
  DibHeader* dib_header_;
};

}  // namespace bitmap
