#include <c-api/parser.h>
#include <libfts/parser.hpp>

extern "C" {
ParserConfig* parser_config_new(const char* path) {
    try {
        auto* ptr = new fts::ParserConfig(fts::load_parser_config(path));
        auto* config = reinterpret_cast<ParserConfig*>(ptr);
        return config;
    } catch (const fts::ConfigException& ex) {
        return nullptr;
    }
}

void parser_config_delete(ParserConfig* config) {
    auto* ptr = reinterpret_cast<fts::ParserConfig*>(config);
    delete ptr;
}
}