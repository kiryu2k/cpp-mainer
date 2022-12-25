#include <bitmap/bitmap.hpp>

#include <cxxopts.hpp>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

bitmap::Pixel to_grayscale(bitmap::Pixel p) {
  const std::uint8_t c = 0.299 * p.red_ + 0.587 * p.green_ + 0.114 * p.blue_;
  return {c, c, c};
}

int main(int argc, char** argv) {
  cxxopts::Options options("sqrt");

  // clang-format off
  options.add_options()
    ("in", "truecolor bmp", cxxopts::value<std::string>())
    ("out", "grayscale bmp", cxxopts::value<std::string>());
  // clang-format on

  const auto result = options.parse(argc, argv);

  if (result.count("in") != 1 || result.count("out") != 1) {
    std::cout << options.help() << "\n";
    return 0;
  }

  const auto in_img_path = result["in"].as<std::string>();
  const auto out_img_path = result["out"].as<std::string>();

  // TODO: make RAII wrapper.
  int fd = open(in_img_path.c_str(), O_RDONLY);
  const auto in_file_size = std::filesystem::file_size(in_img_path);
  char* data =
      static_cast<char*>(mmap(0, in_file_size, PROT_READ, MAP_PRIVATE, fd, 0));

  bitmap::BitmapImage input_img(data, in_file_size);

  std::vector<char> output_img_data(input_img.size_);

  std::copy(
      input_img.data_,
      input_img.data_ + input_img.header_->pixel_offset_,
      output_img_data.begin());

  const auto pixels_out = reinterpret_cast<bitmap::Pixel*>(
      output_img_data.data() + input_img.header_->pixel_offset_);

  std::transform(
      input_img.pixels_begin(),
      input_img.pixels_end(),
      pixels_out,
      to_grayscale);

  std::ofstream out(out_img_path, std::ios::binary);
  out.write(output_img_data.data(), output_img_data.size());

  munmap(data, in_file_size);
  close(fd);
}
