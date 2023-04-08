package main

/*
#cgo CFLAGS: -I${SRCDIR}/../src
#cgo LDFLAGS: -Wl,-rpath,${SRCDIR}
#cgo LDFLAGS: -L${SRCDIR}
#cgo LDFLAGS: -lfts-c
#include <libfts-c/searcher.h>

#include <stdlib.h>
*/
import "C"
import "unsafe"

type ParserConfiguration struct {
	obj *C.ParserConfiguration
}

func ParserConfigurationNew(configPath string) ParserConfiguration {
	var parserConfig ParserConfiguration
	parserConfig.obj = C.parser_configuration_new(C.CString(configPath))
	return parserConfig
}

func (parserConfig ParserConfiguration) ParserConfigurationDelete() {
	C.parser_configuration_delete(parserConfig.obj)
}

type SearcherConfiguration struct {
	obj *C.SearcherConfiguration
}

func SearcherConfigurationNew(configPath string) SearcherConfiguration {
	var searcherConfig SearcherConfiguration
	cStr := C.CString(configPath)
	searcherConfig.obj = C.searcher_configuration_new(cStr)
	C.free(unsafe.Pointer(cStr))
	return searcherConfig
}

func (searcherConfig SearcherConfiguration) SearcherConfigurationDelete() {
	C.searcher_configuration_delete(searcherConfig.obj)
}

type TextIndexAccessor struct {
	obj *C.TextIndexAccessor
}

func TextIndexAccessorNew(indexDir string, parserConfig ParserConfiguration) TextIndexAccessor {
	var textIndexAccessor TextIndexAccessor
	cStr := C.CString(indexDir)
	textIndexAccessor.obj = C.text_index_accessor_new(cStr, parserConfig.obj)
	C.free(unsafe.Pointer(cStr))
	return textIndexAccessor
}

func (textIndexAccessor TextIndexAccessor) TextIndexAccessorLoadDocument(id uint) *C.char {
	return C.text_index_accessor_load_document(textIndexAccessor.obj, C.ulong(id))
}

func makeGoString(document *C.char) string {
	return C.GoString(document)
}

func freeDocument(document *C.char) {
	C.delete_document(document)
}

func (textIndexAccessor TextIndexAccessor) TextIndexAccessorDelete() {
	C.text_index_accessor_delete(textIndexAccessor.obj)
}

type Results struct {
	obj *C.Results
}

func search(query string, textIndexAccessor TextIndexAccessor, searcherConfig SearcherConfiguration) Results {
	var results Results
	cStr := C.CString(query)
	results.obj = C.search(cStr, textIndexAccessor.obj, searcherConfig.obj)
	C.free(unsafe.Pointer(cStr))
	return results
}

func getResultsSize(searchResults Results) uint {
	size := C.get_results_size(searchResults.obj)
	return uint(size)
}

type Result struct {
	score      float64
	documentID uint
}

func searchResultGet(searchResults Results, idx uint) Result {
	resultPtr := C.search_result_get(searchResults.obj, C.ulong(idx))
	return Result{score: float64(resultPtr.score_), documentID: uint(resultPtr.document_id_)}
}
