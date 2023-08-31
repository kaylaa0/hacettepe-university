/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;

public class Main {
    public static void main(String[] args) {
        Main program = new Main();
        program.start(args);
    }

    public void start(String[] args) {
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("output.txt")))); // The
                                                                                                                      // writer
                                                                                                                      // for
                                                                                                                      // output
                                                                                                                      // file.
            System.out.println("Q1 output:\n");
            writer.write("Q1 output:\n\n");
            System.out.println("A-C\nC-J\nC-D\nD-B\nB-H\nH-I\nH-G\nJ-E\nH-F\n");
            writer.write("A-C\nC-J\nC-D\nD-B\nB-H\nH-I\nH-G\nJ-E\nH-F\n\n");
            System.out.println("Q2 output:\n");
            writer.write("Q2 output:\n\n");
            System.out.println("Worst-case graph for eager version:\n" + new Graph(10, 0).toString());
            writer.write("Worst-case graph for eager version:\n" + new Graph(10, 0).toString() + "\n");
            System.out.println("Worst-case graph for lazy version:\n" + new Graph(10, 1).toString());
            writer.write("Worst-case graph for lazy version:\n" + new Graph(10, 1).toString() + "\n");
            System.out.println("Q3 output:\n");
            writer.write("Q3 output:\n\n");
            writer.write(new Maze() + "\n");
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to output file.");
        }
    }
}
