/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.ArrayList;
import java.util.List;

public class Entry {
    public String customer;
    public String type;
    public String date;
    public List<Item> items = new ArrayList<Item>();
    public Float total;

    public void setItems(String[] parts) {
        for (int i = 3; i <= parts.length - 2; i = i + 2) {
            this.items.add(((i - 1) / 2) - 1, new Item(parts[i], parts[i + 1]));
        }
    }

    public Entry(String line) {
        String[] parts = line.split("\\t");
        this.customer = parts[0];
        this.type = parts[1];
        this.date = parts[2];
        this.total = Float.parseFloat("0");
        setItems(parts);
    }
}
