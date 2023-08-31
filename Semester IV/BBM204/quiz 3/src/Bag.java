/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.HashMap;

public class Bag extends HashMap<Integer, Integer> {
    // If you put same item in the bag you will just increase the number,
    // meaning if a vertex which is neighbour of another vertex gets added to it's
    // neighbour list,
    // it will just increase number stating that the edges between them increased.
    public void add(Integer item) {
        if (this.get(item) == null) {
            this.put(item.intValue(), 1);
        } else {
            this.put(item.intValue(), this.get(item.intValue()) + 1);
        }
    }
}
