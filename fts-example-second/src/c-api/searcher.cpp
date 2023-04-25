#include <c-api/searcher.h>
#include <libfts/searcher.hpp>

#include <cstring>

extern "C" {
SearchResults*
search(const char* query, ParserConfig* config, const char* path) {
    auto* ptr = reinterpret_cast<fts::ParserConfig*>(config);
    auto result = fts::search(query, *ptr, path);
    auto* ptr_result = reinterpret_cast<SearchResults*>(
        new fts::SearchResults(std::move(result)));
    return ptr_result;
}

void search_results_delete(SearchResults* ptr) {
    auto* result = reinterpret_cast<fts::SearchResults*>(ptr);
    delete result;
}

SearchResult* search_result_get(SearchResults* ptr, size_t index) {
    auto* results = reinterpret_cast<fts::SearchResults*>(ptr);
    auto* result = reinterpret_cast<SearchResult*>(&results->at(index));
    return result;
}

size_t get_search_result_count(SearchResults* ptr) {
    auto count = reinterpret_cast<fts::SearchResults*>(ptr)->size();
    return count;
}
}