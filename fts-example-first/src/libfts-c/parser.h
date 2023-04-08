#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ParserConfiguration ParserConfiguration;

ParserConfiguration* parser_configuration_new(const char* config_path);

void parser_configuration_delete(ParserConfiguration* parser_config);

#ifdef __cplusplus
}
#endif