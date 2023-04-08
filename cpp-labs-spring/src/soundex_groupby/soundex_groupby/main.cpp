#include <libcsc/soundex/soundex.hpp>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <fast-cpp-csv-parser/csv.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <map>

using Hash = std::string;
using Name = std::string;
using Names = std::vector<Name>;
using SoundexHashMap = std::map<Hash, Names>;

int main(int argc, char **argv) {
    CLI::App app{"soundex groupby app"};
    std::filesystem::path input;
    std::filesystem::path output;
    app.add_option<std::filesystem::path>("-i,--input", input, "input csv file")
        ->required();
    app.add_option<std::filesystem::path>(
           "-o,--output", output, "output json file")
        ->required();
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &ex) {
        return app.exit(ex);
    }
    const std::size_t column_count = 1;
    io::CSVReader<column_count> csv(input);
    csv.set_header("name");
    csv.read_header(io::ignore_extra_column, "name");
    SoundexHashMap hash_map;
    Name name;
    while (csv.read_row(name)) {
        try {
            hash_map[csc::soundex::soundex_hash(name)].push_back(name);
        } catch (const csc::soundex::SoundexHashException &ex) {
            std::cout << ex.what();
            return 1;
        }
    }
    nlohmann::json json(hash_map);
    std::ofstream output_file(output);
    output_file << json;
    return 0;
}