#include <libsolver/sqrt.hpp>

#include <cxxopts.hpp>

#include <iostream>

int main(int argc, char** argv) {
  cxxopts::Options options("sqrt");

  try {
    // clang-format off
    options.add_options()
      ("value", "Non-negative number", cxxopts::value<double>());
    // clang-format on

    const auto result = options.parse(argc, argv);

    if (result.count("value") != 1) {
      std::cout << options.help() << "\n";
      return 0;
    }

    const auto value = result["value"].as<double>();

    std::cout << solver::sqrt(value) << "\n";
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
