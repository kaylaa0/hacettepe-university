/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.Scanner;

public class Vendor1 extends Vendor {
    private String from;
    private String to;
    private String serviceType;
    private Integer estimatedHour;
    private Integer estimatedTotal;
    private boolean confirm;

    @Override
    public void makeReservation() {
        Scanner scan = new Scanner(System.in);
        System.out.println("*********************Vendor1 Interface**************************");
        this.from = scan.next();
        this.to = scan.next();
        this.serviceType = scan.next();
    }
}
