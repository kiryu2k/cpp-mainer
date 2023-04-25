import libfts


def run_searcher():
    config = libfts.api.parser_config_new("ParserConfig.json".encode("utf-8"))
    path = "index".encode("utf-8")
    while True:
        query = input("> ")
        if (query == "exit."):
            break
        if (query == ""):
            continue
        query = query.encode("utf-8")
        results = libfts.api.search(query, config, path)
        result_count = libfts.api.get_search_result_count(results)
        for i in range(result_count):
            res = libfts.api.search_result_get(results, i).contents
            print("{}\t{}\t{}".format(
                res.document_id_, res.score_, str(res.document_.decode("utf-8"))))
        libfts.api.search_results_delete(results)
    libfts.api.parser_config_delete(config)


if __name__ == "__main__":
    run_searcher()
