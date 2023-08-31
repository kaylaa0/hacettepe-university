/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Graph {

    private int size;
    private Bag<Edge>[] graph;

    public Graph(int size, int mode) { // mode 0 for eager, 1 for lazy
        this.size = size;
        this.graph = (Bag<Edge>[]) new Bag[size];
        for (int v = 0; v < this.size; v++)
            graph[v] = new Bag<Edge>();
        this.generate(size, mode);
    }

    public void generate(int size, int mode) {
        if (mode == 0) {// Generating complete graph with decreasing weights for eager version.
            int weight = this.size;
            for (int current = 0; current < this.size; current++) {
                for (int adding = this.size - 1; adding > current; adding--) {
                    Edge temp = new Edge(current, adding, weight);
                    this.addEdge(temp);
                }
                weight--;
            }
        } else if (mode == 1) {// Generating complete graph with same weight for lazy version.
            for (int current = 0; current < this.size; current++) {
                for (int adding = this.size - 1; adding > current; adding--) {
                    Edge temp = new Edge(current, adding, 1);
                    this.addEdge(temp);
                }
            }
        } else {
            System.err.println("Undefined graph mode.");
        }
    }

    public void addEdge(Edge e) {
        int v = e.either(), w = e.other(v);
        graph[v].add(e);
        graph[w].add(e);
    }

    public int vertices() {
        return this.size;
    }

    public Iterable<Edge> adjacent(int vertex) {
        return this.graph[vertex];
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        for (int vertex = 0; vertex < vertices(); vertex++) {
            stringBuilder.append(vertex).append(": ");
            for (Edge neighbor : adjacent(vertex)) {
                stringBuilder.append(neighbor).append(" ");
            }
            stringBuilder.append("\n");
        }
        return stringBuilder.toString();
    }
}
