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

/*
If you want to change analysis accuracy, go to MyHashTable.analyseSearch()
If key doesn't exist everything happens smoothly; at the output file the line:
"Key found with..." changes to:
"Key couldn't be found with...".
Also specific errors are printed to System.err
If there is no space to put in assuming tricky alpha values; it won't put the object in and print specific errors to System.err
After when searching for a key that is not put the separateChaining might have the object while others give error.
 */

public class Main {
    public MyHashTable separateChaining; // My hash tables.
    public MyHashTable linearProbing;
    public MyHashTable doubleHashing;

    public static void main(String[] args) {
        // This way I don't need to write static for every method.
        Main program = new Main();
        program.start(args);
    }

    public void start(String[] args) {
        this.readFile(args); // First we read the input file.
        this.printHashTables(args); // Then we write the output file.
        /*
         * This part is for debugging, I also keep it here to ensure my code works.
         * Check for your self if it actually finds the Employee object.
         * System.out.println(this.separateChaining.get(Integer.parseInt(args[3])));
         * System.out.println(this.linearProbing.get(Integer.parseInt(args[3])));
         * System.out.println(this.doubleHashing.get(Integer.parseInt(args[3])));
         */
    }

    // Just to be sure; checking input and formatting it.
    private String inputCheck(String filename) {
        if (filename.startsWith("<")) {
            filename = filename.replace("<", "");
            filename = filename.replace(">", "");
        }
        return filename;
    }

    // Getting length of input file. Not counting invalid lines.
    private int getLength(List<String> input) {
        int total = 0;
        for (int i = 1; i < input.size(); i++) {
            if (input.get(i).equals("")) {
                continue;
            } else if (input.get(i).equals(" ")) {
                continue;
            } else if (input.get(i).equals("\n")) {
                continue;
            } else if (input.get(i).equals("\t")) {
                continue;
            } else if (input.get(i).equals("\r")) {
                continue;
            } else if (input.get(i).equals("\r\n")) {
                continue;
            } else {
                total++;
            }
        }
        return total;
    }

    private void readFile(String[] args) {
        // My reading method.
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(inputCheck(args[0])))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        this.separateChaining = new MyHashTable(this.getLength(result), Double.parseDouble(args[1]), 1); // The hash
                                                                                                         // tables are
                                                                                                         // initialized
                                                                                                         // by
                                                                                                         // calculated
                                                                                                         // input file
                                                                                                         // length
                                                                                                         // according
                                                                                                         // alpha values
                                                                                                         // and search
                                                                                                         // modes.
        this.linearProbing = new MyHashTable(this.getLength(result), Double.parseDouble(args[2]), 2);
        this.doubleHashing = new MyHashTable(this.getLength(result), Double.parseDouble(args[2]), 3);

        for (int i = 1; i < result.size(); i++) {
            // The input is iterated again. Ignoring first line.
            if (result.get(i).equals("")) {
                continue;
            } else if (result.get(i).equals(" ")) {
                continue;
            }
            String[] parts = result.get(i).split(" ");
            Employee newEmployee = new Employee(parts[0], parts[1], Integer.parseInt(parts[2])); // Creating a new
                                                                                                 // Employee if the line
                                                                                                 // is valid.
            this.separateChaining.put(newEmployee.getPhone(), newEmployee); // Using put method to put in newEmployee in
                                                                            // to hash tables, as the key value phone
                                                                            // number of employee is used.
            this.linearProbing.put(newEmployee.getPhone(), newEmployee);
            this.doubleHashing.put(newEmployee.getPhone(), newEmployee);
        }
    }

    private void printHashTables(String args[]) {
        String[] parts = args[0].split("\\."); // Getting .txt out of the input filename.
        String filename = parts[0] + ",LF1=" + args[1] + ",LF2=" + args[2] + "," + args[3]; // Creating output file
                                                                                            // name.
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(filename + ".txt")))); // The writer for output
                                                                                                // file.
            // Printing out the according output from assigment paper.
            writer.write(filename + "\n");
            writer.write("PART1\n");
            this.printSeparateChaining(writer);
            writer.write("PART2\n");
            writer.write("Hashtable for Linear Probing\n");
            this.printLinearProbing(writer);
            writer.write("Hashtable for Double Hashing\n");
            this.printDoubleHashing(writer);

            // This is a special method returns the required comparison amount and a very
            // precisely calculated average run time.
            long[] results = this.separateChaining.analyseSearch(Integer.parseInt(args[3]));
            Employee testingExistence = separateChaining.get(Integer.parseInt(args[3]));
            if (testingExistence == null) {
                writer.write("SEPARATE CHAINING:\n");
                writer.write("Key couldn't be found with " + results[1] + " comparisons\n");
                writer.write("CPU time taken to search = " + results[0] + ".0 ns\n");
            } else {
                writer.write("SEPARATE CHAINING:\n");
                writer.write("Key found with " + results[1] + " comparisons\n");
                writer.write("CPU time taken to search = " + results[0] + ".0 ns\n");
            }

            long[] results2 = this.linearProbing.analyseSearch(Integer.parseInt(args[3]));
            Employee testingExistence2 = linearProbing.get(Integer.parseInt(args[3]));
            if (testingExistence2 == null) {
                writer.write("LINEAR PROBING:\n");
                writer.write("Key couldn't be found with " + results2[1] + " comparisons\n");
                writer.write("CPU time taken to search = " + results2[0] + ".0 ns\n");
            } else {
                writer.write("LINEAR PROBING:\n");
                writer.write("Key found with " + results2[1] + " comparisons\n");
                writer.write("CPU time taken to search = " + results2[0] + ".0 ns\n");
            }
            long[] results3 = this.doubleHashing.analyseSearch(Integer.parseInt(args[3]));
            Employee testingExistence3 = linearProbing.get(Integer.parseInt(args[3]));
            if (testingExistence3 == null) {
                writer.write("DOUBLE HASHING:\n");
                writer.write("Key couldn't be found with " + results3[1] + " comparisons\n");
                writer.write("CPU time taken to search = " + results3[0] + ".0 ns\n");
            } else {
                writer.write("DOUBLE HASHING:\n");
                writer.write("Key found with " + results3[1] + " comparisons\n");
                writer.write("CPU time taken to search = " + results3[0] + ".0 ns\n");
            }

            writer.close(); // Closing file after printing is done.
        } catch (IOException e) {
            System.err.println("Problem writing to the file: " + filename + ".txt"); // An error just in case.
        }
    }

    private void printSeparateChaining(Writer writer) {
        // When we are printing separate chaining hash table we use this function.
        try {
            for (int each = 0; each < this.separateChaining.TABLE_SIZE; each++) {
                writer.write("[Chain " + each + "]: ");
                this.printChain(writer, each);
            }
        } catch (IOException e) {
            System.err.println("Problem writing separate chaining table to the file.");
        }
    }

    private void printChain(Writer writer, int index) {
        // Iterating chain for printing the hashtable.
        try {
            if (this.separateChaining.EmployeeData[index] == null) {
                writer.write("Null");
            } else {
                writer.write(String.valueOf(this.separateChaining.EmployeeData[index].getFirst().getData().getPhone()));
                Node<Employee> before = this.separateChaining.EmployeeData[index].getFirst();
                Node<Employee> current = this.separateChaining.EmployeeData[index].getFirst().getNext();
                while (current != null) {
                    writer.write("---->" + current.getData().getPhone());
                    before = current;
                    current = current.getNext();
                }
            }
            writer.write("\n");

        } catch (IOException e) {
            System.err.println("Problem writing separate chaining table to the file.");
        }
    }

    private void printLinearProbing(Writer writer) {
        // When we are printing linear probing hash table we use this function.
        try {
            for (int each = 0; each < this.linearProbing.TABLE_SIZE; each++) {
                writer.write("[" + each + "]--->");
                if (this.linearProbing.EmployeeData[each] == null) {
                    writer.write("null");
                } else {
                    writer.write(String.valueOf(this.linearProbing.EmployeeData[each].getFirst().getData().getPhone()));
                }
                writer.write("\n");
            }
        } catch (IOException e) {
            System.err.println("Problem writing linear probing table to the file.");
        }
    }

    private void printDoubleHashing(Writer writer) {
        // When we are printing double hashing hash table we use this function.
        try {
            for (int each = 0; each < this.doubleHashing.TABLE_SIZE; each++) {
                writer.write("[" + each + "]--->");
                if (this.doubleHashing.EmployeeData[each] == null) {
                    writer.write("null");
                } else {
                    writer.write(String.valueOf(this.doubleHashing.EmployeeData[each].getFirst().getData().getPhone()));
                }
                writer.write("\n");
            }
        } catch (IOException e) {
            System.err.println("Problem writing double hashing table to the file.");
        }
    }
}
