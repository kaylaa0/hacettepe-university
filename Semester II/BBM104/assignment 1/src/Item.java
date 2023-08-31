/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Item {
    public String name;
    public String amount;
    public Float price;

    public Item(String name, String amount) {
        this.name = name;
        this.amount = amount;
        this.price = Float.parseFloat("0");
    }

    public void setPrice(String i) {
        this.price = Float.parseFloat(i);
    }
}
