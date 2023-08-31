/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class DiGraph extends Graph {
    public DiGraph(String filename) {
        super(filename);
    }

    // Everything is same except when we add an edge we don't add it both ways.
    @Override
    public void addEdge(int v, int w) {
        this.get(v).add(w);
    }
}
