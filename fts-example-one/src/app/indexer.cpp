#include <libfts/indexer.hpp>

#include <cxxopts.hpp>
#include <rapidcsv.h>

#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options("fts: indexer");
    try {
        options.add_options()("c,csv", "csv", cxxopts::value<std::string>())(
                "i,index", "index", cxxopts::value<std::string>())(
                "config",
                "config",
                cxxopts::value<std::string>()->default_value("config.json"));
        const auto opt_result = options.parse(argc, argv);
        const auto csv_path = opt_result["csv"].as<std::string>();
        const auto index_dir = opt_result["index"].as<std::string>();
        const auto config_path = opt_result["config"].as<std::string>();
        const auto config = fts::ParserConfiguration::load_configuration_file(
                config_path);
        fts::IndexBuilder index_builder;
        fts::TextIndexWriter text_index_writer;
        rapidcsv::Document doc(csv_path);
        const auto book_IDs = doc.GetColumn<std::size_t>("bookID");
        const auto titles = doc.GetColumn<std::string>("title");
        const auto langs = doc.GetColumn<std::string>("language_code");
        if (titles.size() != langs.size()) {
            std::cerr << "invalid read csv file" << std::endl;
            return 1;
        }
        for (std::size_t i = 0; i < titles.size(); ++i) {
            if (langs[i] == "eng") {
                index_builder.add_document(book_IDs[i], titles[i], config);
                if ((i + 1) % 1000 == 0) {
                    std::cout << "added " << i + 1 << " documents..."
                              << std::endl;
                }
            }
        }
        text_index_writer.write(index_dir, index_builder.index());
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const fts::ConfigurationException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const fts::IndexException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}