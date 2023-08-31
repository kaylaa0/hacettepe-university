/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.Scanner;

public class Vendor2 extends Vendor {
    private String from;
    private String to;
    private String serviceType;
    private Integer estimatedHour;
    private Integer estimatedTotal;
    private boolean confirm;
    private Transport[] transports;

    @Override
    public void makeReservation() {
        Scanner scan = new Scanner(System.in);
        System.out.println("Vendor 2");
    }
}
