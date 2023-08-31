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
    public Simulator sim = new Simulator();

    public static void main(String[] args) {
        Main program = new Main(); // Calling main non-static methods.
        program.start(args);
    }

    public void start(String[] args) {
        this.readFile(args[0]); // Reading input file
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(args[1]))));
            // Writing to output file
            writer.write(sim.graph.toString());
            writer.write(sim.startSimulation());
            writer.write(sim.passMessageSimulation());
            writer.write(sim.rootCalculation());
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to " + args[1]);
            e.printStackTrace();
        }
    }

    private int getGraphSize(String[] parts) {
        // Reading first line and dividing number of elements by 2 to get graph sizea.
        int size = 0;
        for (int i = 0; i < parts.length; i++) {
            if (parts[i].equals("")) {
                continue;
            } else if (parts[i].equals(" ")) {
                continue;
            } else if (parts[i].equals("\n")) {
                continue;
            }
            size++;
        }
        return size / 2;
    }

    public Graph generateGraph(String[] parts) {
        // Creating basic graph structure.
        int size = this.getGraphSize(parts);
        Graph temp = new Graph(size);
        for (int i = 0; i < parts.length; i = i + 2) {
            temp.setNode(new Node(parts[i], Integer.parseInt(parts[i + 1])), i / 2);
        }
        return temp;
    }

    public void readFile(String fileName) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(fileName))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) input.txt");
        }
        String[] firstLine = result.get(0).split(" ");
        sim.setGraph(this.generateGraph(firstLine));
        sim.setTree(this.generateGraph(firstLine));
        sim.setStartNode(sim.graph.getNode(result.get(1)));
        // From first 2 lines creating basic graph structure.
        for (int i = 2; i < result.size(); i++) {
            // Rest of the lines converted in to edges and added to graph.
            if (result.get(i).equals("")) {
                continue;
            } else if (result.get(i).equals(" ")) {
                continue;
            } else if (result.get(i).equals("\n")) {
                continue;
            }
            String[] parts = result.get(i).split(" ");
            sim.graph.addEdge(new Edge(parts[0], parts[1]));
        }
    }

}
