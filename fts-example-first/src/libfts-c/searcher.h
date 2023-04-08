#pragma once

#include <libfts-c/parser.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SearcherConfiguration SearcherConfiguration;

SearcherConfiguration* searcher_configuration_new(const char* config_path);

void searcher_configuration_delete(SearcherConfiguration* searcher_config);

typedef struct TextIndexAccessor TextIndexAccessor;

TextIndexAccessor*
text_index_accessor_new(const char* index_dir, ParserConfiguration* config);

const char* text_index_accessor_load_document(
        TextIndexAccessor* text_index_accessor, size_t id);

void delete_document(char* document);

void text_index_accessor_delete(TextIndexAccessor* text_index_accessor);

typedef struct Results Results;

Results*
search(const char* query,
       TextIndexAccessor* text_index_accessor,
       SearcherConfiguration* searcher_config);

size_t get_results_size(Results* search_results);

typedef struct Result {
    double score_;
    size_t document_id_;
} Result;

Result* search_result_get(Results* search_results, size_t idx);

#ifdef __cplusplus
}
#endif