#include <libfts-c/searcher.h>
#include <libfts/searcher.hpp>

#include <cstring>

extern "C" {
SearcherConfiguration* searcher_configuration_new(const char* config_path)
{
    try {
        return reinterpret_cast<SearcherConfiguration*>(
                new fts::SearcherConfiguration(
                        fts::SearcherConfiguration::load_configuration_file(
                                config_path)));
    } catch (const fts::ConfigurationException& e) {
        return nullptr;
    }
}

void searcher_configuration_delete(SearcherConfiguration* searcher_config)
{
    delete reinterpret_cast<fts::SearcherConfiguration*>(searcher_config);
}

TextIndexAccessor*
text_index_accessor_new(const char* index_dir, ParserConfiguration* config)
{
    try {
        auto* text_index_accessor = new fts::TextIndexAccessor(
                index_dir,
                *reinterpret_cast<fts::ParserConfiguration*>(config));
        return reinterpret_cast<TextIndexAccessor*>(text_index_accessor);
    } catch (const fts::AccessorException& e) {
        return nullptr;
    } catch (const std::filesystem::__cxx11::filesystem_error& e) {
        return nullptr;
    }
}

const char* text_index_accessor_load_document(
        TextIndexAccessor* text_index_accessor, size_t id)
{
    auto document
            = reinterpret_cast<fts::TextIndexAccessor*>(text_index_accessor)
                      ->load_document(id);
    return strdup(document.c_str());
}

void delete_document(char* document)
{
    free(document);
}

void text_index_accessor_delete(TextIndexAccessor* text_index_accessor)
{
    delete reinterpret_cast<fts::TextIndexAccessor*>(text_index_accessor);
}

Results*
search(const char* query,
       TextIndexAccessor* text_index_accessor,
       SearcherConfiguration* searcher_config)
{
    auto results = fts::Searcher::search(
            query,
            *reinterpret_cast<fts::TextIndexAccessor*>(text_index_accessor),
            *reinterpret_cast<fts::SearcherConfiguration*>(searcher_config));
    return reinterpret_cast<Results*>(new fts::Results(std::move(results)));
}

size_t get_results_size(Results* search_results)
{
    auto* results = reinterpret_cast<fts::Results*>(search_results);
    return results->size();
}

Result* search_result_get(Results* search_results, size_t idx)
{
    auto* results = reinterpret_cast<fts::Results*>(search_results);
    auto* result = &results->at(idx);
    return reinterpret_cast<Result*>(result);
}
}