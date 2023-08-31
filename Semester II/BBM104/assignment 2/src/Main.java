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
    public static List<Personnel> personnels = new ArrayList<Personnel>();;

    private void readFile(String[] args) {
        readPersonnelFile(args[0]);
        readMonitorFile(args[1]);
    }

    private void readPersonnelFile(String filename) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(filename))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        // For each line we create according class of personnel and add it to general
        // list.
        for (int i = 0; i < result.size(); i++) {
            String[] parts = result.get(i).split("\\t");
            if (parts[2].equals("CHIEF")) {
                personnels.add(new Chief(parts[0], parts[1], parts[2], parts[3]));
            } else if (parts[2].equals("FACULTY_MEMBER")) {
                personnels.add(new Faculty(parts[0], parts[1], parts[2], parts[3]));
            } else if (parts[2].equals("WORKER")) {
                personnels.add(new Worker(parts[0], parts[1], parts[2], parts[3]));
            } else if (parts[2].equals("SECURITY")) {
                personnels.add(new Security(parts[0], parts[1], parts[2], parts[3]));
            } else if (parts[2].equals("OFFICER")) {
                personnels.add(new Officer(parts[0], parts[1], parts[2], parts[3]));
            } else if (parts[2].equals("PARTTIME_EMPLOYEE")) {
                personnels.add(new PartTime(parts[0], parts[1], parts[2], parts[3]));
            } else if (parts[2].equals("RESEARCH_ASSISTANT")) {
                personnels.add(new Assistants(parts[0], parts[1], parts[2], parts[3]));
            } else {
                System.out.println("Invalid personnel position.");
            }
        }
    }

    private void readMonitorFile(String filename) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(filename))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        for (int i = 0; i < result.size(); i++) {
            String[] parts = result.get(i).split("\\t");
            getPersonnel(parts[0]).setWorkHours(Integer.parseInt(parts[1]), Integer.parseInt(parts[2]),
                    Integer.parseInt(parts[3]), Integer.parseInt(parts[4]));
        }
    }

    // Search general personnel list and when found a registration match return.
    private Personnel getPersonnel(String registration) {
        for (int i = 0; i < personnels.size(); i++) {
            if (personnels.get(i).registration.equals(registration)) {
                return personnels.get(i);
            }
        }
        return null;
    }

    public static void main(String[] args) {
        Main program = new Main();
        program.start(args);
    }

    private void calculateAll() {
        for (Integer i = 0; i < personnels.size(); i++) {
            personnels.get(i).calculateSalary();
        }
    }

    // This is a debugging print, instead of printing to files it prints console.
    private void printPersonnels() {
        personnels.forEach(System.out::println);
    }

    private void printToFile() {
        personnels.forEach(this::printToFile);
    }

    private void printToFile(Personnel personnel) {
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(personnel.registration + ".txt"))));
            writer.write(personnel.toString());
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + personnel.registration);
        }
    }

    public void start(String[] args) {
        this.readFile(args);
        calculateAll();
        printToFile();
    }
}
