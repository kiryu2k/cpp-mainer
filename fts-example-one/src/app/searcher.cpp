#include <libfts/searcher.hpp>

#include <cxxopts.hpp>
#include <replxx.hxx>

#include <iostream>

void print_search_result(
        const std::string& query,
        const fts::IndexAccessor& index_accessor,
        const fts::SearcherConfiguration& searcher_config)
{
    const auto result
            = fts::Searcher::search(query, index_accessor, searcher_config);
    for (std::size_t i = 0; i < result.size(); ++i) {
        std::cout << i + 1 << "\t" << result[i].score_ << "\t"
                  << result[i].document_id_ << "\t"
                  << index_accessor.load_document(result[i].document_id_)
                  << std::endl;
    }
}

static void interactive_search(
        const fts::IndexAccessor& index_accessor,
        const fts::SearcherConfiguration& searcher_config)
{
    const char* prompt = "> ";
    replxx::Replxx rxx;
    rxx.clear_screen();
    for (;;) {
        char const* input = nullptr;
        do {
            input = rxx.input(prompt);
        } while ((input == nullptr) && (errno == EAGAIN));
        if (input == nullptr) {
            break;
        }
        // change cinput into a std::string
        // easier to manipulate
        const std::string input_query{input};
        if (input_query.empty()) {
            // user hit enter on an empty line
            continue;
        }
        if (input_query.compare(0, 5, ".quit") == 0
            || input_query.compare(0, 5, ".exit") == 0) {
            // exit the repl
            break;
        }
        print_search_result(input_query, index_accessor, searcher_config);
    }
}

int main(int argc, char** argv)
{
    cxxopts::Options options("fts: searcher");
    try {
        options.add_options()(
                "i,index", "index", cxxopts::value<std::string>())(
                "q,query",
                "query",
                cxxopts::value<std::string>()->no_implicit_value())(
                "config",
                "config",
                cxxopts::value<std::string>()->default_value("config.json"));
        const auto opt_result = options.parse(argc, argv);
        const auto index_dir = opt_result["index"].as<std::string>();
        const auto config_path = opt_result["config"].as<std::string>();
        const auto parser_config
                = fts::ParserConfiguration::load_configuration_file(
                        config_path);
        fts::TextIndexAccessor text_index_accessor(index_dir, parser_config);
        const auto searcher_config
                = fts::SearcherConfiguration::load_configuration_file(
                        config_path);
        if (opt_result.count("query") == 1) {
            const auto query = opt_result["query"].as<std::string>();
            print_search_result(query, text_index_accessor, searcher_config);
        } else {
            interactive_search(text_index_accessor, searcher_config);
        }
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const fts::ConfigurationException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const fts::IndexException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const fts::AccessorException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}