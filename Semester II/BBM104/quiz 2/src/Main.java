/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    // Main charts for sport tournament.
    public static List<Hockey> hockeyChart = new ArrayList<Hockey>();
    public static List<Handball> handballChart = new ArrayList<Handball>();
    public static List<Basketball> basketballChart = new ArrayList<Basketball>();
    public static List<Volleyball> volleyballChart = new ArrayList<Volleyball>();

    private static void readFile(String[] args) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(args[0]))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        for (int i = 0; i < result.size(); i++) {
            String[] parts = result.get(i).split("\\t");
            // The line put in the process with correct chart according to its first letter.
            if (parts[0].equals("I")) {
                processMatch(hockeyChart, parts[1], parts[2], parts[3], parts[0]);
            } else if (parts[0].equals("H")) {
                processMatch(handballChart, parts[1], parts[2], parts[3], parts[0]);
            } else if (parts[0].equals("B")) {
                processMatch(basketballChart, parts[1], parts[2], parts[3], parts[0]);
            } else if (parts[0].equals("V")) {
                processMatch(volleyballChart, parts[1], parts[2], parts[3], parts[0]);
            }
        }
    }

    private static void addNew(String name, String type) {
        // How can instantiate generic ? Very complex answers to that one, this method
        // is easier.
        if (type.equals("I")) {
            hockeyChart.add(new Hockey(name));
        } else if (type.equals("H")) {
            handballChart.add(new Handball(name));
        } else if (type.equals("B")) {
            basketballChart.add(new Basketball(name));
        } else if (type.equals("V")) {
            volleyballChart.add(new Volleyball(name));
        }
    }

    private static <E extends Sports> void processMatch(List<E> list, String club1, String club2, String result,
            String type) {
        // Checking if the current teams exist in our list. If any of them do not exist,
        // we first initialize them.
        boolean found1 = false;
        boolean found2 = false;
        for (int x = 0; x < list.size(); x++) {
            if (list.get(x).getClubName().equals(club1)) {
                found1 = true;
            } else if (list.get(x).getClubName().equals(club2)) {
                found2 = true;
            }
        }
        if (!found1) {
            addNew(club1, type);
        }
        if (!found2) {
            addNew(club2, type);
        }
        // Calling result method for the teams with the result.
        for (int x = 0; x < list.size(); x++) {
            if (list.get(x).getClubName().equals(club1)) {
                list.get(x).result(-1, result);
            } else if (list.get(x).getClubName().equals(club2)) {
                list.get(x).result(1, result);
            }
        }
    }

    private static void sortCharts() {
        sortCharts(hockeyChart);
        sortCharts(handballChart);
        sortCharts(basketballChart);
        sortCharts(volleyballChart);
    }

    private static <E extends Sports> void sortCharts(List<E> A) {
        // Some old codes put to use. This reversed cocktail short is my own code from
        // BBM204 a1 of this year. Equality check is a modified version of it.
        cocktailShort(A);
        equalityCheck(A);
    }

    private static <E extends Sports> void cocktailShort(List<E> A) {
        boolean swapped = true;
        int start = 0;
        int limit = A.size();
        while (swapped) {
            swapped = false;
            for (int i = start; i < limit - 1; i++) {
                if (A.get(i).getPoints() < A.get(i + 1).getPoints()) {
                    E temp = A.get(i);
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
                if (A.get(i).getPoints() < A.get(i + 1).getPoints()) {
                    E temp = A.get(i);
                    A.set(i, A.get(i + 1));
                    A.set(i + 1, temp);
                    swapped = true;
                }
            }
            start = start + 1;
        }
    }

    private static <E extends Sports> void equalityCheck(List<E> A) {
        boolean swapped = true;
        int start = 0;
        int limit = A.size();
        while (swapped) {
            swapped = false;
            for (int i = start; i < limit - 1; i++) {
                if (A.get(i).getPoints() == A.get(i + 1).getPoints()) {
                    if (A.get(i).getGoalDifference() < A.get(i + 1).getGoalDifference()) {
                        E temp = A.get(i);
                        A.set(i, A.get(i + 1));
                        A.set(i + 1, temp);
                        swapped = true;
                    }
                }
            }
            if (!swapped)
                break;
            swapped = false;
            limit = limit - 1;
            for (int i = limit - 1; i >= start; i--) {
                if (A.get(i).getPoints() == A.get(i + 1).getPoints()) {
                    if (A.get(i).getGoalDifference() < A.get(i + 1).getGoalDifference()) {
                        E temp = A.get(i);
                        A.set(i, A.get(i + 1));
                        A.set(i + 1, temp);
                        swapped = true;
                    }
                }
            }
            start = start + 1;
        }
    }

    private static void printFile() {
        printFile("iceHockey.txt", hockeyChart);
        printFile("handball.txt", handballChart);
        printFile("basketball.txt", basketballChart);
        printFile("volleyball.txt", volleyballChart);
    }

    private static <E extends Sports> void printFile(String filename, List<E> A) {
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(filename))));
            for (int i = 0; i < A.size(); i++) {
                writer.write(String.valueOf(i + 1) + ".\t");
                writer.write(A.get(i) + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + filename);
        }
    }

    // This is a debugging method. Accessible by public if something goes wrong.
    public static void printScreen() {
        for (int x = 0; x < hockeyChart.size(); x++) {
            System.out.println(hockeyChart.get(x));
        }
        for (int x = 0; x < handballChart.size(); x++) {
            System.out.println(handballChart.get(x));
        }
        for (int x = 0; x < basketballChart.size(); x++) {
            System.out.println(basketballChart.get(x));
        }
        for (int x = 0; x < volleyballChart.size(); x++) {
            System.out.println(volleyballChart.get(x));
        }
    }

    public static void main(String[] args) {
        Handball test = new Handball("test");
    }
}
