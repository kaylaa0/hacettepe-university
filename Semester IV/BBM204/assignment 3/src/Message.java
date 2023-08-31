/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.ArrayList;

public class Message { // A message class, holding current best nodes list, and the value vor quick
                       // comparision.
    public ArrayList<Bottle> bestList = new ArrayList<Bottle>();
    public int bestCapacity;

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        for (Bottle bottle : this.bestList) {
            builder.append(bottle.toString());
        }
        return builder.toString();
    }
}
