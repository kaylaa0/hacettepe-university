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
    public static void main(String[] args) {
        Main program = new Main();
        Stack<Integer> stack = new Stack<Integer>(10); // TA stated in piazza as 10.
        List<String> input = program.readFile(args[0]);
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("octal.txt"))));

            for (String line : input) {
                int currentD = Integer.valueOf(line);
                while (currentD > 0) {
                    int remainder = currentD % 8;
                    stack.Push(remainder);
                    currentD = (currentD - remainder) / 8;
                }
                while (!stack.isEmpty()) {
                    writer.write(String.valueOf(stack.Pop()));
                }
                writer.write("\n");
            }

            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to octal.txt");
            e.printStackTrace();
        }
    }

    public List<String> readFile(String fileName) { // Read file method.
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(fileName))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) " + fileName);
        }
        return result;
    }
}
