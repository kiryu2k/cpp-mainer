import ctypes

api = ctypes.CDLL("client/libc-api.so")

api.parser_config_new.restype = ctypes.c_void_p
api.parser_config_new.argtypes = [ctypes.c_char_p]

api.parser_config_delete.argtypes = [ctypes.c_void_p]

api.search.restype = ctypes.c_void_p
api.search.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_char_p]

api.search_results_delete.argtypes = [ctypes.c_void_p]


class SearchResult(ctypes.Structure):
    _fields_ = [("document_id_", ctypes.c_size_t),
                ("score_", ctypes.c_double),
                ("document_", ctypes.c_char_p)]


api.search_result_get.restype = ctypes.POINTER(SearchResult)
api.search_result_get.argtypes = [ctypes.c_void_p, ctypes.c_size_t]

api.get_search_result_count.restype = ctypes.c_size_t
api.get_search_result_count.argtypes = [ctypes.c_void_p]
