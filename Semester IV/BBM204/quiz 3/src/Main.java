/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.util.*;

public class Main {

    public Graph one; // Graph that will used for question one
    public Graph two; // Graph that will used for question two
    public DiGraph three; // Graph that will used for question three
    public HashMap<Integer, Boolean> markedQ2 = new HashMap<Integer, Boolean>(); // Marked tracer, when going deep in
                                                                                 // graph we mark where we step so we
                                                                                 // can track.
    private HashMap<Integer, Integer> edgeTo = new HashMap<Integer, Integer>(); // Path tracer, when going deep in graph
                                                                                // we map our path so we can track.
    private List<List<Integer>> circleSet = new ArrayList<List<Integer>>(); // For solution of question 2, I first
                                                                            // detect circles in graph and put in this
                                                                            // list. If a vertex is not in any circle it
                                                                            // must be bridge.
    public HashMap<Integer, Integer> question2print = new HashMap<Integer, Integer>(); // Last minute implementation to
                                                                                       // fix output of question 2.

    private void generateGraphs(String[] args) {
        // Graphs can be created by a filename, they will read the file, check
        // Graphs.java for more information.
        this.one = new Graph(args[0]);
        this.two = new Graph(args[1]);
        this.three = new DiGraph(args[2]);
    }

    // Debugger function, no use.
    public void printGraph(Graph graph) {
        for (int i = 0; i < graph.size(); i++) {
            System.out.println(graph.get(i));
        }
    }

    // Main function that will called. It prints results as well as calling them.
    public void printOutput() {
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("output.txt"))));
            writer.write("Q1\n" +
                    solveQ1() + "\n" +
                    "Q2\n");
            solveQ2(writer);
            writer.write("\nQ3\n");
            if (solveQ3()) {
                writer.write("True");
            } else if (!solveQ3()) {
                writer.write("False");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to the file: output.txt");
        }
    }

    public Integer solveQ1() {
        // We check every vertex and it's neighbour's, if it has more than 1 connection
        // with the neighbour we add it minus 1 to total, at last we divide by 2. That
        // is the number of parallel edges.
        Integer total = 0;
        for (Map.Entry<Integer, Bag> vertex : one.entrySet()) {
            for (Map.Entry<Integer, Integer> edge : vertex.getValue().entrySet()) {
                if (one.get(vertex.getKey()).get(edge.getKey()).compareTo(1) > 0) {
                    total = total + one.get(vertex.getKey()).get(edge.getKey()) - 1;
                }
            }
        }
        return total / 2;
    }

    public void solveQ2(Writer writer) {
        // For question 2 we first find all circles in the list, if a vertex is not in a
        // circle it is a bridge.
        for (Map.Entry<Integer, Bag> vertex : two.entrySet()) {
            // For all vertices we go dfs.
            clearMarked();
            edgeTo.clear();
            dfs(vertex.getKey());
        }
        // This is to check if we found one, if not we print required result.
        boolean foundOne = false;
        for (Map.Entry<Integer, Bag> vertex : two.entrySet()) {
            if (!isInCircle(vertex.getKey())) {
                foundOne = true;
                // This iterable list is the neighbours of bridge vertex. We will print it
                // accordingly.
                Iterable<Integer> result = two.adj(vertex.getKey());
                Iterator<Integer> iter = result.iterator();
                while (iter.hasNext()) {
                    question2print.put(iter.next(), vertex.getKey());
                }
            }
        }
        if (foundOne) {
            try {
                for (Map.Entry<Integer, Integer> vertex : question2print.entrySet()) {
                    if (vertex.getValue().compareTo(vertex.getKey()) < 0) {
                        writer.write(vertex.getValue() + "-" + vertex.getKey() + " ");
                    } else {
                        writer.write(vertex.getKey() + "-" + vertex.getValue() + " ");
                    }
                }
            } catch (IOException e) {
                System.err.println("Problem writing to the file 2: output.txt");
            }
        }
        if (!foundOne) {
            // If not found other print.
            try {
                writer.write("Graph is two-edge connected");
            } catch (IOException e) {
                System.err.println("Problem writing to the file 2: output.txt");
            }
        }
    }

    // This is to clear our path for before going every dfs.
    public void clearMarked() {
        for (Map.Entry<Integer, Bag> vertex : two.entrySet()) {
            markedQ2.put(vertex.getKey(), false);
        }
    }

    // This is same function but used for question 3.
    public void clearMarked3() {
        for (Map.Entry<Integer, Bag> vertex : three.entrySet()) {
            markedQ2.put(vertex.getKey(), false);
        }
    }

    // After we found the circles of graph, we check if every vertex is in graph
    // with this function.
    private boolean isInCircle(Integer key) {
        for (Integer x = 0; x < circleSet.size(); x++) {
            for (Integer y = 0; y < circleSet.get(x).size(); y++) {
                if (key.equals(circleSet.get(x).get(y))) {
                    return true;
                }
            }
        }
        return false;
    }

    // When we found a circle we add it to general circles list with this function
    private void addCircle(Integer last, Integer circle) {
        circleSet.add(new ArrayList<>());

        Integer next = last;
        while (edgeTo.get(next) != null) {
            circleSet.get(circleSet.size() - 1).add(next);
            next = edgeTo.get(next);
        }
        circleSet.get(circleSet.size() - 1).add(circle);
    }

    // dfs function for question 2.
    private void dfs(int v) {
        this.markedQ2.put(v, true);
        for (Integer w : two.adj(v))
            if (!w.equals(v)) {
                if (markedQ2.get(w)) {
                    if (!edgeTo.get(w).equals(v)) {
                        addCircle(w, v);
                    } else {

                    }
                } else if (!markedQ2.get(w)) {
                    edgeTo.put(v, w);
                    dfs(w);
                    markedQ2.put(w, false);
                    edgeTo.remove(w);
                }
            }
        this.markedQ2.put(v, false);
    }

    public boolean solveQ3() {
        // For question 3 we go dfs and try to reach a Hamiltonian path. If we do it
        // return true to all the way, if not at the end it returns false.
        boolean found = false;
        for (Map.Entry<Integer, Bag> vertex : three.entrySet()) {
            clearMarked3();
            edgeTo.clear();
            if (dfs3(vertex.getKey())) {
                found = true;
            }
        }
        return found;
    }

    // Function to check if current path a Hamiltonian path. Simple logic is if
    // every vertex marked (stepped on), it is Hamiltonian path.
    private boolean isHamlithon() {
        for (Integer x = 0; x < markedQ2.size(); x++) {
            if (!markedQ2.get(x)) {
                return false;
            }
        }
        return true;
    }

    // dfs function for question 3 customized to check Hamiltonian paths.
    private boolean dfs3(int v) {
        this.markedQ2.put(v, true);
        for (Integer w : three.adj(v)) {
            if (!markedQ2.get(w)) {
                markedQ2.put(w, true);
                if (isHamlithon()) {
                    return true;
                }
                markedQ2.put(w, false);
                edgeTo.put(v, w);
                if (dfs3(w)) {
                    return true;
                }
                markedQ2.put(w, false);
                edgeTo.remove(w);
            }
        }
        this.markedQ2.put(v, false);
        return false;
    }

    public static void main(String[] args) {
        // Main function
        Main program = new Main();
        program.start(args);
    }

    public void start(String[] args) {
        this.generateGraphs(args);
        this.printOutput();
    }
}
