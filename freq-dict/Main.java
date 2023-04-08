import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Scanner;

public class Main {
    private static ArrayList<String> testWords = new ArrayList<>(
            Arrays.asList("the", "of", "and", "to", "in", "a", "is", "that", "for", "it"));

    static void containsPutGetTest(ArrayList<String> words) {
        System.out.println("CONTAINS + PUT + GET:");
        HashMap<String, Integer> wordFreq = new HashMap<>();
        long elapsedSeconds = 0;
        long startTime = System.nanoTime();
        for (var word : words) {
            if (wordFreq.containsKey(word)) {
                wordFreq.put(word, wordFreq.get(word) + 1);
            } else {
                wordFreq.put(word, 1);
            }
        }
        elapsedSeconds = System.nanoTime() - startTime;
        System.out.println("elapsed time: " + elapsedSeconds * 1e-9 + "s");
        // for (var word : testWords) {
        // System.out.println(wordFreq.get(word) + " " + word);
        // }
        System.out.println("--------------------------------------------");
    }

    static void getOrDefaultTest(ArrayList<String> words) {
        System.out.println("GET OR DEFAULT:");
        HashMap<String, Integer> wordFreq = new HashMap<>();
        long elapsedSeconds = 0;
        long startTime = System.nanoTime();
        for (var word : words) {
            wordFreq.put(word, wordFreq.getOrDefault(word, 0) + 1);
        }
        elapsedSeconds = System.nanoTime() - startTime;
        System.out.println("elapsed time: " + elapsedSeconds * 1e-9 + "s");
        // for (var word : testWords) {
        // System.out.println(wordFreq.get(word) + " " + word);
        // }
        System.out.println("--------------------------------------------");
    }

    static void mergeTest(ArrayList<String> words) {
        System.out.println("MERGE:");
        HashMap<String, Integer> wordFreq = new HashMap<>();
        long elapsedSeconds = 0;
        long startTime = System.nanoTime();
        for (var word : words) {
            wordFreq.merge(word, 1, (prev, one) -> prev + one);
        }
        elapsedSeconds = System.nanoTime() - startTime;
        System.out.println("elapsed time: " + elapsedSeconds * 1e-9 + "s");
        // for (var word : testWords) {
        // System.out.println(wordFreq.get(word) + " " + word);
        // }
        System.out.println("--------------------------------------------");
    }

    public static void main(String[] args) {
        try {
            var scanner = new Scanner(new File("words-5m.txt"));
            ArrayList<String> words = new ArrayList<>();
            while (scanner.hasNext()) {
                words.add(scanner.next());
            }
            scanner.close();
            containsPutGetTest(words);
            getOrDefaultTest(words);
            mergeTest(words);
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            throw new RuntimeException(e);
        }
    }
}