#include "robin_hood.h"
#include <absl/container/flat_hash_map.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>

static void robin_hood_hash_map_test(const std::vector<std::string> &words) {
    std::cout << "ROBIN HOOD HASH MAP:\n";
    robin_hood::unordered_flat_map<std::string, std::size_t> word_freq;
    const auto start_time = std::chrono::steady_clock::now();
    for (const auto &word : words) {
        ++word_freq[word];
    }
    const std::chrono::duration<double> elapsed_seconds =
        std::chrono::steady_clock::now() - start_time;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    // std::vector<std::string> test_words = {
    //     "the", "of", "and", "to", "in", "a", "is", "that", "for", "it"};
    // for (const auto &word : test_words) {
    //     std::cout << word_freq[word] << " " << word << "\n";
    // }
    std::cout << "-------------------------------------------------------\n";
}

static void unordered_map_test(const std::vector<std::string> &words) {
    std::cout << "STD UNORDERED MAP:\n";
    std::unordered_map<std::string, std::size_t> word_freq;
    const auto start_time = std::chrono::steady_clock::now();
    for (const auto &word : words) {
        ++word_freq[word];
    }
    const std::chrono::duration<double> elapsed_seconds =
        std::chrono::steady_clock::now() - start_time;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    // std::vector<std::string> test_words = {
    //     "the", "of", "and", "to", "in", "a", "is", "that", "for", "it"};
    // for (const auto &word : test_words) {
    //     std::cout << word_freq[word] << " " << word << "\n";
    // }
    std::cout << "-------------------------------------------------------\n";
}

static void absl_flat_hash_map_test(const std::vector<std::string> &words) {
    std::cout << "ABSL FLAT HASH MAP:\n";
    absl::flat_hash_map<std::string, std::size_t> word_freq;
    const auto start_time = std::chrono::steady_clock::now();
    for (const auto &word : words) {
        ++word_freq[word];
    }
    const std::chrono::duration<double> elapsed_seconds =
        std::chrono::steady_clock::now() - start_time;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    // std::vector<std::string> test_words = {
    //     "the", "of", "and", "to", "in", "a", "is", "that", "for", "it"};
    // for (const auto &word : test_words) {
    //     std::cout << word_freq[word] << " " << word << "\n";
    // }
    std::cout << "-------------------------------------------------------\n";
}

int main() {
    std::ifstream file("words-5m.txt");
    std::istream_iterator<std::string> start(file), end;
    std::vector<std::string> words(start, end);
    unordered_map_test(words);
    robin_hood_hash_map_test(words);
    absl_flat_hash_map_test(words);
    return 0;
}