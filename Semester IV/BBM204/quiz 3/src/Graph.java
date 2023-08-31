/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

// Graph is a hashmap that points to a bag.
public class Graph extends HashMap<Integer, Bag> {
    private int V;

    // Graphs can be created with a filename.
    public Graph(String filename) {
        this.readFile(filename);
    }

    // Function to read file in to a graph.
    public void readFile(String filename) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(inputCheck(filename)))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
        }
        for (int i = 0; i < result.size(); i++) {
            if (i == 0) {
                // First line is used to create Bags.
                this.V = Integer.parseInt(result.get(i));
                for (int v = 0; v < V; v++) {
                    this.put(v, new Bag());
                }
            } else if (i == 1) {
                continue;
            } else {
                String[] parts = result.get(i).split(" ");
                // After line 2, we call add edge method.
                this.addEdge(Integer.parseInt(parts[0]), Integer.parseInt(parts[1]));
            }
        }
    }

    // This is not used.
    public Graph(int V) {
        this.V = V;
        for (int v = 0; v < V; v++)
            this.put(v, new Bag());
    }

    // Add edge function.
    public void addEdge(int v, int w) {
        this.get(v).add(w);
        this.get(w).add(v);
    }

    // I didn't know if args was going to be <input1.txt> or input1.txt, this is
    // just to make sure.
    private String inputCheck(String filename) {
        if (filename.startsWith("<")) {
            filename = filename.replace("<", "");
            filename = filename.replace(">", "");
        }
        return filename;
    }

    // adj function return neighbours of a vertex.
    public Iterable<Integer> adj(Integer v) {
        return this.get(v).keySet();
    }

}
