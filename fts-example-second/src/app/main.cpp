#include <libfts/searcher.hpp>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <fmt/core.h>
#include <rapidcsv.h>
#include <replxx.hxx>

static void start_interactive_search(
    const fts::ParserConfig& parser_config,
    const std::string& index_path) {
    replxx::Replxx readliner;
    readliner.clear_screen();
    while (true) {
        char const* cinput{nullptr};
        do {
            cinput = readliner.input("> ");
        } while ((cinput == nullptr) && (errno == EAGAIN));
        if (cinput == nullptr) {
            break;
        }
        std::string query = cinput;
        if (query.empty()) {
            continue;
        }
        const auto& query_results =
            fts::search(query, parser_config, index_path);
        for (const auto& query_result : query_results) {
            fmt::print(
                "{}\t{}\t{}\n",
                query_result.document_id_,
                query_result.score_,
                query_result.document_);
        }
    }
}

int main(int argc, char** argv) {
    CLI::App app{"full text searcher"};
    std::string query;
    std::string config_path = "ParserConfig.json";
    std::string index_path;
    std::string data_path;

    auto* indexer = app.add_subcommand("index");
    indexer->add_option<std::string>("-i,--index", index_path)->required();
    indexer->add_option<std::string>("--csv", data_path)->required();
    indexer->add_option<std::string>("-c,--config", config_path);

    auto* searcher = app.add_subcommand("search");
    searcher->add_option<std::string>("-i,--index", index_path)->required();
    searcher->add_option<std::string>("-q,--query", query);
    searcher->add_option<std::string>("-c,--config", config_path);

    try {
        CLI11_PARSE(app, argc, argv);
    } catch (const CLI::ParseError& e) {
        std::cerr << e.what() << '\n';
        return 0;
    }

    fts::ParserConfig parser_config;

    try {
        parser_config = fts::load_parser_config(config_path);
    } catch (const fts::ConfigException& e) {
        std::cerr << e.what() << '\n';
        return 0;
    }

    if (indexer->parsed()) {
        rapidcsv::Document books_file(data_path);
        const auto books = books_file.GetColumn<std::string>("title");
        const auto books_identifiers =
            books_file.GetColumn<std::size_t>("bookID");
        const auto books_languages =
            books_file.GetColumn<std::string>("language_code");

        std::unordered_set<fts::Document> books_set;
        fts::IndexBuilder builder;
        const auto books_count = books.size();
        for (std::size_t i = 0; i < books_count; ++i) {
            if (books_set.find(books[i]) == books_set.end() &&
                books_languages[i] == "eng") {
                builder.add_document(
                    books_identifiers[i], books[i], parser_config);
                books_set.insert(books[i]);
            }
        }
        fts::TextIndexWriter::write(index_path, builder.index());
        fmt::print("Indexing is completed\n");
    }

    if (searcher->parsed()) {
        if (searcher->count("--query") == 1) {
            const auto& query_results =
                fts::search(query, parser_config, index_path);
            for (const auto& query_result : query_results) {
                fmt::print(
                    "{}\t{}\t{}\n",
                    query_result.document_id_,
                    query_result.score_,
                    query_result.document_);
            }
            return 0;
        }
        start_interactive_search(parser_config, index_path);
    }
}