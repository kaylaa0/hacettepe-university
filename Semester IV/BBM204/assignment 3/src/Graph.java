/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Graph {

    private int size; // Basic Graph
    private Node[] graph;

    public Graph(int size) {
        this.size = size;
        this.graph = (Node[]) new Node[size];
    }

    public void addEdge(Edge e) {
        String v = e.either(), w = e.other(v);
        this.getNode(v).adjecencyList.add(w);
        this.getNode(w).adjecencyList.add(v);
    }

    public void setNode(Node n, int pos) {
        this.graph[pos] = n;
    }

    public Node getNode(String name) {
        for (Node node : this.graph) {
            if (node.name.equals(name)) {
                return node;
            }
        }
        return null;
    }

    public Node getNode(int i) {
        return this.graph[i];
    }

    public int getSize() {
        return size;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder("Graph structure:\n");
        for (Node node : this.graph) {
            stringBuilder.append(node);
        }
        return stringBuilder.toString();
    }
}
