/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Bottle { // Used in message to store nodes.
    public String nodeName;
    public int capacity;

    public Bottle(String nodeName, int capacity) {
        this.nodeName = nodeName;
        this.capacity = capacity;
    }

    @Override
    public String toString() {
        return "[" + this.nodeName + "," + this.capacity + "]";
    }
}
