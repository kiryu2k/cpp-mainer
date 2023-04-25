#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ParserConfig ParserConfig;

ParserConfig* parser_config_new(const char* path);

void parser_config_delete(ParserConfig* config);

#ifdef __cplusplus
}
#endif