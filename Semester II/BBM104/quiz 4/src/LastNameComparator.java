/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.Comparator;

public class LastNameComparator implements Comparator<Contact> {

    @Override
    public int compare(Contact c1, Contact c2) {
        String v1 = c1.getLastName();
        String v2 = c2.getLastName();
        return v1.compareTo(v2);
    }
}
