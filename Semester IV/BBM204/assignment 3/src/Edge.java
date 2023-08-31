/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Edge { // Basic edge class
    private final String v, w;

    public Edge(String v, String w) {
        this.v = v;
        this.w = w;
    }

    public String either() {
        return v;
    }

    public String other(String vertex) {
        if (vertex.equals(v))
            return w;
        else
            return v;
    }

    @Override
    public String toString() {
        return this.v + "-" + this.w;
    }
}
