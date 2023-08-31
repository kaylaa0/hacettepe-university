/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Product {

    public String name;
    public String type;
    public String dateFrom;
    public String dateTo;
    public String price;

    public Product(String line) {
        String[] parts = line.split("\\t");
        this.name = parts[0];
        this.type = parts[1];
        this.dateFrom = parts[2];
        this.dateTo = parts[3];
        this.price = parts[4];
    }

}
