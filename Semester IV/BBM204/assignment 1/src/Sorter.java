/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.util.Scanner;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.ArrayList;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.Random;
import java.text.SimpleDateFormat;
import java.util.Calendar;

public class Sorter {

    public static boolean exit;
    public static List<Integer> list = new ArrayList<Integer>();

    static public String getTime() {
        Calendar cal = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy_MM_dd_HH_mm_ss");
        return sdf.format(cal.getTime());
    }

    static public void cocktail() {
        Sorter.cocktail(10);
    }

    static public void cocktail(Scanner scan) {
        System.out.println("How many times you want to run analysis? ");
        int times = Integer.parseInt(scan.nextLine());
        Sorter.cocktail(times);
    }

    static public void cocktail(Writer writer) {
        try {
            System.out.println("Sorting 4096 items.");
            Sorter.readFile("4096");
            writer.write(Sorter.cocktail(list) + "\n");
            System.out.println("Sorting 16384 items.");
            Sorter.readFile("16384");
            writer.write(Sorter.cocktail(list) + "\n");
            System.out.println("Sorting 65536 items.");
            Sorter.readFile("65536");
            writer.write(Sorter.cocktail(list) + "\n");
            System.out.println("Sorting 131072 items.");
            Sorter.readFile("131072");
            writer.write(Sorter.cocktail(list) + "\n");
            System.out.println("Sorting 262144 items.");
            Sorter.readFile("262144");
            writer.write(Sorter.cocktail(list) + "\n");
            System.out.println("Sorting 524288 items.");
            Sorter.readFile("524288");
            writer.write(Sorter.cocktail(list) + "\n");
        } catch (IOException e) {
            System.err.println("Problem writing to log file, cocktail sort.");
        }
    }

    static public void cocktail(int times) {
        String logName = getTime() + ".log";
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(logName))));
            writer.write(
                    "RESULTS OF ANALYSIS FOR COCKTAIL SHAKER ALGORITHM WITH GENERATED LISTS " + times + " TIMES\n");
            writer.write("------------------------------------------------------------------------------\n");
            System.out.println("RUNNING ANALYSIS ON COCKTAIL SHAKER ALGORITHM " + times + " TIMES");
            for (int i = 0; i < times; i++) {
                System.out.println("Cycle " + (i + 1));
                writer.write("Iteration " + (i + 1) + ":\n");
                /*
                 * System.out.println("Generating new set of files.");
                 * Sorter.genFile();
                 */
                System.out.println("Beginning sorting operation.");
                Sorter.cocktail(writer);
            }
            System.out.println("Operation complete.");
            writer.write("---END OF ANALYSIS---\n");
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem creating the file: " + logName);
        } catch (IOException e) {
            System.err.println("Problem writing to log file: " + logName);
        }
    }

    static public long cocktail(List<Integer> A) {
        long startTime = System.nanoTime();
        boolean swapped = true;
        int start = 0;
        int limit = A.size();
        while (swapped) {
            swapped = false;
            for (int i = start; i < limit - 1; i++) {
                if (A.get(i) > A.get(i + 1)) {
                    int temp = A.get(i);
                    A.set(i, A.get(i + 1));
                    A.set(i + 1, temp);
                    swapped = true;
                }
            }
            if (!swapped)
                break;
            swapped = false;
            limit = limit - 1;
            for (int i = limit - 1; i >= start; i--) {
                if (A.get(i) > A.get(i + 1)) {
                    int temp = A.get(i);
                    A.set(i, A.get(i + 1));
                    A.set(i + 1, temp);
                    swapped = true;
                }
            }
            start = start + 1;
        }
        long endTime = System.nanoTime();
        return endTime - startTime;
    }

    static public void pigeon() {
        Sorter.pigeon(10);
    }

    static public void pigeon(Scanner scan) {
        System.out.println("How many times you want to run analysis? ");
        int times = Integer.parseInt(scan.nextLine());
        Sorter.pigeon(times);
    }

    static public void pigeon(Writer writer) {
        try {
            System.out.println("Sorting 4096 items.");
            Sorter.readFile("4096");
            writer.write(Sorter.pigeon(list) + "\n");
            System.out.println("Sorting 16384 items.");
            Sorter.readFile("16384");
            writer.write(Sorter.pigeon(list) + "\n");
            System.out.println("Sorting 65536 items.");
            Sorter.readFile("65536");
            writer.write(Sorter.pigeon(list) + "\n");
            System.out.println("Sorting 131072 items.");
            Sorter.readFile("131072");
            writer.write(Sorter.pigeon(list) + "\n");
            System.out.println("Sorting 262144 items.");
            Sorter.readFile("262144");
            writer.write(Sorter.pigeon(list) + "\n");
            System.out.println("Sorting 524288 items.");
            Sorter.readFile("524288");
            writer.write(Sorter.pigeon(list) + "\n");
        } catch (IOException e) {
            System.err.println("Problem writing to log file, pigeon sort.");
        }
    }

    static public void pigeon(Integer times) {
        String logName = getTime() + ".log";
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(logName))));
            writer.write("RESULTS OF ANALYSIS FOR PIGEONHOLE ALGORITHM WITH GENERATED LISTS " + times + " TIMES\n");
            writer.write("-------------------------------------------------------------------------\n");
            System.out.println("RUNNING ANALYSIS ON PIGEONHOLE ALGORITHM " + times + " TIMES");
            for (int i = 0; i < times; i++) {
                System.out.println("Cycle " + (i + 1));
                writer.write("Iteration " + (i + 1) + ":\n");
                /*
                 * System.out.println("Generating new set of files.");
                 * Sorter.genFile();
                 */
                System.out.println("Beginning sorting operation.");
                Sorter.pigeon(writer);
            }
            System.out.println("Operation complete.");
            writer.write("---END OF ANALYSIS---\n");
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem creating the file: " + logName);
        } catch (IOException e) {
            System.err.println("Problem writing to log file: " + logName);
        }
    }

    static public long pigeon(List<Integer> A) {
        long startTime = System.nanoTime();
        int min = A.get(0);
        int max = A.get(0);
        int range;
        for (int i = 1; i < A.size(); i++) {
            if (A.get(i) < min)
                min = A.get(i);
            if (A.get(i) > max)
                max = A.get(i);
        }
        range = max - min + 1;
        int[] holes = new int[range];
        for (Integer integer : A)
            holes[integer - min]++;
        int x = 0;
        for (int count = 0; count < range; count++)
            while (holes[count]-- > 0) {
                list.set(x, count + min);
                x++;
            }
        long endTime = System.nanoTime();
        return endTime - startTime;
    }

    static public void bitonic() {
        Sorter.bitonic(10);
    }

    static public void bitonic(Scanner scan) {
        System.out.println("How many times you want to run analysis? ");
        int times = Integer.parseInt(scan.nextLine());
        System.out.println("Please state direction of bitonic sorting.");
        int direction = Integer.parseInt(scan.nextLine());
        Sorter.bitonic(times, direction);
    }

    static public void bitonic(Writer writer) {
        try {
            System.out.println("Sorting 4096 items.");
            Sorter.readFile("4096");
            writer.write(Sorter.bitonic(list) + "\n");
            System.out.println("Sorting 16384 items.");
            Sorter.readFile("16384");
            writer.write(Sorter.bitonic(list) + "\n");
            System.out.println("Sorting 65536 items.");
            Sorter.readFile("65536");
            writer.write(Sorter.bitonic(list) + "\n");
            System.out.println("Sorting 131072 items.");
            Sorter.readFile("131072");
            writer.write(Sorter.bitonic(list) + "\n");
            System.out.println("Sorting 262144 items.");
            Sorter.readFile("262144");
            writer.write(Sorter.bitonic(list) + "\n");
            System.out.println("Sorting 524288 items.");
            Sorter.readFile("524288");
            writer.write(Sorter.bitonic(list) + "\n");
        } catch (IOException e) {
            System.err.println("Problem writing to log file, pigeon sort.");
        }
    }

    static public void bitonic(Writer writer, Integer direction) {
        try {
            System.out.println("Sorting 4096 items.");
            Sorter.readFile("4096");
            writer.write(Sorter.bitonic(list, direction) + "\n");
            System.out.println("Sorting 16384 items.");
            Sorter.readFile("16384");
            writer.write(Sorter.bitonic(list, direction) + "\n");
            System.out.println("Sorting 65536 items.");
            Sorter.readFile("65536");
            writer.write(Sorter.bitonic(list, direction) + "\n");
            System.out.println("Sorting 131072 items.");
            Sorter.readFile("131072");
            writer.write(Sorter.bitonic(list, direction) + "\n");
            System.out.println("Sorting 262144 items.");
            Sorter.readFile("262144");
            writer.write(Sorter.bitonic(list, direction) + "\n");
            System.out.println("Sorting 524288 items.");
            Sorter.readFile("524288");
            writer.write(Sorter.bitonic(list, direction) + "\n");
        } catch (IOException e) {
            System.err.println("Problem writing to log file, pigeon sort.");
        }
    }

    static public void bitonic(Integer times) {
        String logName = getTime() + ".log";
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(logName))));
            writer.write("RESULTS OF ANALYSIS FOR BITONIC ALGORITHM WITH GENERATED LISTS " + times + " TIMES\n");
            writer.write("----------------------------------------------------------------------\n");
            System.out.println("RUNNING ANALYSIS ON BITONIC ALGORITHM " + times + " TIMES");
            for (int i = 0; i < times; i++) {
                System.out.println("Cycle " + (i + 1));
                writer.write("Iteration " + (i + 1) + ":\n");
                /*
                 * System.out.println("Generating new set of files.");
                 * Sorter.genFile();
                 */
                System.out.println("Beginning sorting operation.");
                Sorter.bitonic(writer);
            }
            System.out.println("Operation complete.");
            writer.write("---END OF ANALYSIS---\n");
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem creating the file: " + logName);
        } catch (IOException e) {
            System.err.println("Problem writing to log file: " + logName);
        }
    }

    static public void bitonic(Integer times, Integer direction) {
        String logName = getTime() + ".log";
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(logName))));
            writer.write("RESULTS OF ANALYSIS FOR BITONIC ALGORITHM WITH GENERATED LISTS " + times + " TIMES\n");
            writer.write("----------------------------------------------------------------------\n");
            System.out.println("RUNNING ANALYSIS ON BITONIC ALGORITHM " + times + " TIMES");
            for (int i = 0; i < times; i++) {
                System.out.println("Cycle " + (i + 1));
                writer.write("Iteration " + (i + 1) + ":\n");
                /*
                 * System.out.println("Generating new set of files.");
                 * Sorter.genFile();
                 */
                System.out.println("Beginning sorting operation.");
                Sorter.bitonic(writer, direction);
            }
            System.out.println("Operation complete.");
            writer.write("---END OF ANALYSIS---\n");
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem creating the file: " + logName);
        } catch (IOException e) {
            System.err.println("Problem writing to log file: " + logName);
        }
    }

    static public long bitonic(List<Integer> A, Integer direction) {
        long startTime = System.nanoTime();
        Sorter.bitonicSort(A, A.size(), direction);
        long endTime = System.nanoTime();
        return endTime - startTime;
    }

    static public long bitonic(List<Integer> A) {
        Scanner scan = new Scanner(System.in);
        System.out.println("Please state direction of bitonic sorting.");
        int direction = Integer.parseInt(scan.nextLine());
        long startTime = System.nanoTime();
        Sorter.bitonicSort(A, A.size(), direction);
        long endTime = System.nanoTime();
        return endTime - startTime;
    }

    static public void compAndSwap(List<Integer> a, int i, int j, int dire) {
        if ((a.get(i) > a.get(j) && dire == 1) || (a.get(i) < a.get(j) && dire == 0)) {
            int temp = a.get(i);
            a.set(i, a.get(j));
            a.set(j, temp);
        }
    }

    static public void bitonicMerge(List<Integer> a, int low, int cnt, int dire) {
        if (cnt > 1) {
            int k = cnt / 2;
            for (int i = low; i < low + k; i++)
                compAndSwap(a, i, i + k, dire);
            bitonicMerge(a, low, k, dire);
            bitonicMerge(a, low + k, k, dire);
        }
    }

    static public void bitonicSort(List<Integer> a, int low, int cnt, int dire) {
        if (cnt > 1) {
            int k = cnt / 2;

            bitonicSort(a, low, k, 1);
            bitonicSort(a, low + k, k, 0);
            bitonicMerge(a, low, cnt, dire);
        }
    }

    static public void bitonicSort(List<Integer> a, int N, int up) {
        bitonicSort(a, 0, N, up);
    }

    static public void comb() {
        Sorter.comb(10);
    }

    static public void comb(Scanner scan) {
        System.out.println("How many times you want to run analysis? ");
        int times = Integer.parseInt(scan.nextLine());
        Sorter.comb(times);
    }

    static public void comb(Writer writer) {
        try {
            System.out.println("Sorting 4096 items.");
            Sorter.readFile("4096");
            writer.write(Sorter.comb(list) + "\n");
            System.out.println("Sorting 16384 items.");
            Sorter.readFile("16384");
            writer.write(Sorter.comb(list) + "\n");
            System.out.println("Sorting 65536 items.");
            Sorter.readFile("65536");
            writer.write(Sorter.comb(list) + "\n");
            System.out.println("Sorting 131072 items.");
            Sorter.readFile("131072");
            writer.write(Sorter.comb(list) + "\n");
            System.out.println("Sorting 262144 items.");
            Sorter.readFile("262144");
            writer.write(Sorter.comb(list) + "\n");
            System.out.println("Sorting 524288 items.");
            Sorter.readFile("524288");
            writer.write(Sorter.comb(list) + "\n");
        } catch (IOException e) {
            System.err.println("Problem writing to log file, pigeon sort.");
        }
    }

    static public void comb(Integer times) {
        String logName = getTime() + ".log";
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(logName))));
            writer.write("RESULTS OF ANALYSIS FOR COMB SORT ALGORITHM WITH GENERATED LISTS " + times + " TIMES\n");
            writer.write("------------------------------------------------------------------------\n");
            System.out.println("RUNNING ANALYSIS ON COMB SORT ALGORITHM " + times + " TIMES");
            for (int i = 0; i < times; i++) {
                System.out.println("Cycle " + (i + 1));
                writer.write("Iteration " + (i + 1) + ":\n");
                /*
                 * System.out.println("Generating new set of files.");
                 * Sorter.genFile();
                 */
                System.out.println("Beginning sorting operation.");
                Sorter.comb(writer);
            }
            System.out.println("Operation complete.");
            writer.write("---END OF ANALYSIS---\n");
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem creating the file: " + logName);
        } catch (IOException e) {
            System.err.println("Problem writing to log file: " + logName);
        }
    }

    static public long comb(List<Integer> A) {
        long startTime = System.nanoTime();
        int gap = A.size();
        double shrink = 1.3;
        boolean sorted = false;
        while (!sorted) {
            gap = (int) (gap / shrink);
            if (gap <= 1) {
                gap = 1;
                sorted = true;
            }
            int i = 0;
            while (i + gap < A.size()) {
                if (A.get(i) > A.get(i + gap)) {
                    Sorter.swapKeys(A, i, i + gap);
                    sorted = false;
                }
                i++;
            }
        }
        long endTime = System.nanoTime();
        return endTime - startTime;
    }

    static public long combReversed(List<Integer> A) {
        long startTime = System.nanoTime();
        int gap = 1;
        double shrink = 1.3;
        boolean sorted = false;
        int iter = 0;
        int ghost = 1;
        while (!sorted) {
            ghost = gap;
            gap = (int) (gap * shrink);
            if (gap == ghost) {
                if (iter != 0) {
                    gap++;
                }
            }

            if (gap >= A.size()) {
                gap = A.size();
                sorted = true;
            }
            int i = 0;
            while (i + gap < A.size()) {
                Sorter.swapKeys(A, i, i + gap);
                sorted = false;
                i++;
            }
            iter++;
        }
        long endTime = System.nanoTime();
        return endTime - startTime;
    }

    static public void swapKeys(List<Integer> A, int i, int j) {
        int temp;
        temp = A.get(i);
        A.set(i, A.get(j));
        A.set(j, temp);
    }

    public static void display() {
        System.out.print("Welcome to Sorter program.\n" +
                "Must provide files containing integers from 0 to 2147483647 or generate them.\n" +
                "Calculations will print in nano seconds.\n" +
                "Result list will be saved in a output file.\n" +
                "Analysis will be add to log file.\n" +
                "1-Run analysis\n" +
                "2-Cocktail Shaker Sort Algorithm\n" +
                "3-Pigeonhole Sort Algorithm\n" +
                "4-Bitonic Sort Algorithm\n" +
                "5-Comb Sort Algorithm\n" +
                "6-Read(Refresh) list file\n" +
                "7-Print currently in memory list file\n" +
                "8-Generate list file\n" +
                "9-Exit\n" +
                "Enter your choose: ");
    }

    public static void saveList() {
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(String.valueOf(list.size())))));
            for (int i = 0; i < list.size(); i++) {
                writer.write(list.get(i) + "\n");
            }
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem saving list." + list.size());
        } catch (IOException e) {
            System.err.println("Problem writing to list file: " + list.size());
        }
    }

    private static void readFile() {
        Sorter.readFile("4096");
    }

    private static void readFile(String filename) {
        list.clear();
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(filename))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        for (int i = 0; i < result.size(); i++) {
            list.add(i, Integer.parseInt(result.get(i)));
        }
    }

    private static void genFile(int count) {
        Random rand = new Random();
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(String.valueOf(count)))));
            for (int i = 0; i < count; i++) {
                writer.write(rand.nextInt(1147483647) + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + count);
        }
    }

    private static void genFile(int count, int range) {
        Random rand = new Random();
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(String.valueOf(count)))));
            for (int i = 0; i < count; i++) {
                writer.write(rand.nextInt(range) + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + count);
        }
    }

    private static void genFile(Scanner scan) {
        System.out.println("Which type of generation you want ?");
        String type = scan.nextLine();
        if (type.equals("best")) {
            Sorter.genFile(type);
        } else if (type.equals("worst")) {
            Sorter.genFile(type);
        } else if (type.equals("small")) {
            Sorter.genFile(type);
        } else if (type.equals("average")) {
            Sorter.genFile();
        } else if (type.equals("same")) {
            Sorter.genFile(type);
        } else if (type.equals("worst_comb")) {
            Sorter.genFile(type);
        } else {
            System.out.println("Invalid type.");
            Sorter.genFile(scan);
        }
    }

    private static void genFileBest() {
        Sorter.genFileBest(4096);
        Sorter.genFileBest(16384);
        Sorter.genFileBest(65536);
        Sorter.genFileBest(131072);
        Sorter.genFileBest(262144);
        Sorter.genFileBest(524288);
    }

    private static void genFileBest(int count) {
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(String.valueOf(count)))));
            for (int i = 1; i <= count; i++) {
                writer.write(i + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + count);
        }
    }

    private static void genFileWorst() {
        Sorter.genFileWorst(4096);
        Sorter.genFileWorst(16384);
        Sorter.genFileWorst(65536);
        Sorter.genFileWorst(131072);
        Sorter.genFileWorst(262144);
        Sorter.genFileWorst(524288);
    }

    private static void genFileWorst(int count) {
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(String.valueOf(count)))));
            for (int i = count; i >= 1; i--) {
                writer.write(i + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + count);
        }
    }

    private static void genWorstComb(int count) {
        Sorter.genFile("best");
        Sorter.readFile("4096");
        Sorter.combReversed(list);
        Sorter.saveList();
        Sorter.readFile("16384");
        Sorter.combReversed(list);
        Sorter.saveList();
        Sorter.readFile("65536");
        Sorter.combReversed(list);
        Sorter.saveList();
        Sorter.readFile("131072");
        Sorter.combReversed(list);
        Sorter.saveList();
        Sorter.readFile("262144");
        Sorter.combReversed(list);
        Sorter.saveList();
        Sorter.readFile("524288");
        Sorter.combReversed(list);
        Sorter.saveList();
    }

    private static void genSame() {
        Sorter.genSame(4096);
        Sorter.genSame(16384);
        Sorter.genSame(65536);
        Sorter.genSame(131072);
        Sorter.genSame(262144);
        Sorter.genSame(524288);
    }

    private static void genSame(Integer count) {
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(String.valueOf(count)))));
            for (int i = 1; i <= count; i++) {
                writer.write(1 + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + count);
        }
    }

    private static void genWorstComb() {
        Sorter.genWorstComb(4096);
        Sorter.genWorstComb(16384);
        Sorter.genWorstComb(65536);
        Sorter.genWorstComb(131072);
        Sorter.genWorstComb(262144);
        Sorter.genWorstComb(524288);
    }

    private static void genFile(String mode) {
        if (mode.equals("best")) {
            Sorter.genFileBest();
        } else if (mode.equals("worst")) {
            Sorter.genFileWorst();
        } else if (mode.equals("same")) {
            Sorter.genSame();
        } else if (mode.equals("small_range")) {
            Sorter.genFile(4096, 100);
            Sorter.genFile(16384, 100);
            Sorter.genFile(65536, 100);
            Sorter.genFile(131072, 100);
            Sorter.genFile(262144, 100);
            Sorter.genFile(524288, 100);
        } else if (mode.equals("worst_comb")) {
            Sorter.genWorstComb();
        }
    }

    private static void genFile() {
        Sorter.genFile(4096);
        Sorter.genFile(16384);
        Sorter.genFile(65536);
        Sorter.genFile(131072);
        Sorter.genFile(262144);
        Sorter.genFile(524288);
    }

    private static void runAnalysis() {
        String logName = getTime() + ".log";
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(logName))));
            writer.write("RESULTS OF ANALYSIS AVERAGE CASES\n");
            writer.write("---------------------------------\n");
            System.out.println("RUNNING ANALYSIS FOR AVERAGE CASES");
            for (int i = 0; i < 5; i++) {
                System.out.println("Cycle " + (i + 1));
                writer.write("Iteration " + (i + 1) + ":\n");
                System.out.println("Generating new set of files.");
                Sorter.genFile();
                System.out.println("Beginning cocktail sorting operation.");
                Sorter.cocktail(writer);
                System.out.println("Beginning pigeon sorting operation.");
                Sorter.pigeon(writer);
                System.out.println("Beginning bitonic sorting operation.");
                Sorter.bitonic(writer, 1);
                System.out.println("Beginning comb sorting operation.");
                Sorter.comb(writer);
            }
            writer.write("RESULTS OF ANALYSIS BEST CASE\n");
            writer.write("---------------------------------\n");
            System.out.println("RUNNING ANALYSIS FOR BEST CASE");
            System.out.println("Generating new set of files.");
            Sorter.genFile("best");
            System.out.println("Beginning cocktail sorting operation.");
            Sorter.cocktail(writer);
            System.out.println("Beginning pigeon sorting operation.");
            Sorter.pigeon(writer);
            System.out.println("Beginning bitonic sorting operation.");
            Sorter.bitonic(writer, 1);
            System.out.println("Beginning comb sorting operation.");
            Sorter.comb(writer);
            writer.write("RESULTS OF ANALYSIS WORST CASE\n");
            writer.write("---------------------------------\n");
            System.out.println("RUNNING ANALYSIS FOR WORST CASE");
            System.out.println("Generating new set of files.");
            Sorter.genFile("worst");
            System.out.println("Beginning cocktail sorting operation.");
            Sorter.cocktail(writer);
            System.out.println("Beginning pigeon sorting operation.");
            Sorter.pigeon(writer);
            System.out.println("Beginning bitonic sorting operation.");
            Sorter.bitonic(writer, 1);
            System.out.println("Beginning comb sorting operation.");
            Sorter.comb(writer);
            writer.write("RESULTS OF ANALYSIS SMALL CASE\n");
            writer.write("---------------------------------\n");
            System.out.println("RUNNING ANALYSIS FOR SMALL CASE");
            System.out.println("Generating new set of files.");
            Sorter.genFile("small");
            System.out.println("Beginning cocktail sorting operation.");
            Sorter.cocktail(writer);
            System.out.println("Beginning pigeon sorting operation.");
            Sorter.pigeon(writer);
            System.out.println("Beginning bitonic sorting operation.");
            Sorter.bitonic(writer, 1);
            System.out.println("Beginning comb sorting operation.");
            Sorter.comb(writer);
            System.out.println("Operation complete.");
            writer.write("---END OF ANALYSIS---\n");
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem creating the file: " + logName);
        } catch (IOException e) {
            System.err.println("Problem writing to log file: " + logName);
        }

    }

    private static void printFile() {
        list.forEach(System.out::println);
    }

    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        String input;
        Sorter.readFile();
        while (!exit) {
            Sorter.display();
            input = scan.nextLine();
            switch (input) {
                case "1":
                    Sorter.runAnalysis();
                    break;
                case "2":
                    Sorter.cocktail(scan);
                    break;
                case "3":
                    Sorter.pigeon(1);
                    break;
                case "4":
                    Sorter.bitonic(1, 1);
                    break;
                case "5":
                    Sorter.comb(1);
                    break;
                case "6":
                    Sorter.readFile();
                    break;
                case "7":
                    Sorter.printFile();
                    scan.nextLine();
                    break;
                case "8":
                    Sorter.genFile(scan);
                    break;
                case "9":
                    exit = true;
                    break;
            }
        }
    }
}
