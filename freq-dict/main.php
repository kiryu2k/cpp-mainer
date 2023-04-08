<?php

function brute_forse_count_test($words) {
    echo "BRUTE FORSE COUNT:\n";
    $word_freq = array();
    foreach ($words as $word_num => $word) {
        $word_freq[$word] = 0;
    }
    $elapsed_seconds = 0;
    $start = microtime(true);
    foreach ($words as $word_num => $word) {
        $word_freq[$word] += 1;
    }
    $elapsed_seconds = microtime(true) - $start;
    echo "elapsed time: " . $elapsed_seconds . "s\n";
    // $test_words = array("the", "of", "and", "to", "in", "a", "is", "that", "for", "it");
    // foreach ($test_words as $word_num => $word) {
    //     echo $word_freq[$word] . " " . $word . "\n";
    // }
    echo "------------------------------------------------\n";
}

function array_count_values_test($words) {
    echo "ARRAY COUNT VALUES FUNC\n";
    $start = microtime(true);
    $word_freq = array_count_values($words);
    $elapsed_seconds = microtime(true) - $start;
    echo "elapsed time: " . $elapsed_seconds . "s\n";
    // $test_words = array("the", "of", "and", "to", "in", "a", "is", "that", "for", "it");
    // foreach ($test_words as $word_num => $word) {
    //     echo $word_freq[$word] . " " . $word . "\n";
    // }
    echo "------------------------------------------------\n";
}

$words = file('words-5m.txt', FILE_IGNORE_NEW_LINES);
brute_forse_count_test($words);
array_count_values_test($words);

?>