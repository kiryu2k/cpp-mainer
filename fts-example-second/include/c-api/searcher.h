#pragma once

#include <c-api/parser.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SearchResults SearchResults;

typedef struct SearchResult {
    size_t document_id_;
    double score_;
    char* document_;
} SearchResult;

SearchResults*
search(const char* query, ParserConfig* config, const char* path);

void search_results_delete(SearchResults* ptr);

SearchResult* search_result_get(SearchResults* ptr, size_t index);

size_t get_search_result_count(SearchResults* ptr);

#ifdef __cplusplus
}
#endif