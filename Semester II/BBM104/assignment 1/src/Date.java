/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Date {
    public Integer day;
    public Integer month;
    public Integer year;

    public Date(String date) {
        String[] parts = date.split("\\.");
        this.day = Integer.parseInt(parts[0]);
        this.month = Integer.parseInt(parts[1]);
        this.year = Integer.parseInt(parts[2]);
    }
}
