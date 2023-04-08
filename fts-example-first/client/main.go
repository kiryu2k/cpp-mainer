package main

import (
	"bufio"
	"fmt"
	"os"
)

func stringResult(searchResults Results, textIndexAccessor TextIndexAccessor) string {
	size := getResultsSize(searchResults)
	var i uint
	var result string
	for ; i < size; i++ {
		resultInfo := searchResultGet(searchResults, i)
		document := textIndexAccessor.TextIndexAccessorLoadDocument(resultInfo.documentID)
		result += fmt.Sprintf("%d\t%f\t%d\t%s\n", i+1, resultInfo.score, resultInfo.documentID, makeGoString(document))
		freeDocument(document)
	}
	return result
}

func main() {
	configPath := "config.json"
	parserConfig := ParserConfigurationNew(configPath)
	searcherConfig := SearcherConfigurationNew(configPath)
	indexDir := "index"
	textIndexAccessor := TextIndexAccessorNew(indexDir, parserConfig)
	for {
		fmt.Print("> ")
		reader := bufio.NewReader(os.Stdin)
		query, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("An error occured while reading input. Please try again", err)
			return
		}
		searchResults := search(query, textIndexAccessor, searcherConfig)
		fmt.Print(stringResult(searchResults, textIndexAccessor))
	}
	textIndexAccessor.TextIndexAccessorDelete()
	searcherConfig.SearcherConfigurationDelete()
	parserConfig.ParserConfigurationDelete()
}
