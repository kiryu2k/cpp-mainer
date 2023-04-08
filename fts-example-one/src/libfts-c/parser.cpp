#include <libfts-c/parser.h>
#include <libfts/parser.hpp>

extern "C" {
ParserConfiguration* parser_configuration_new(const char* config_path)
{
    try {
        return reinterpret_cast<ParserConfiguration*>(
                new fts::ParserConfiguration(
                        fts::ParserConfiguration::load_configuration_file(
                                config_path)));
    } catch (const fts::ConfigurationException& e) {
        return nullptr;
    }
}

void parser_configuration_delete(ParserConfiguration* parser_config)
{
    delete reinterpret_cast<fts::ParserConfiguration*>(parser_config);
}
}