/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.Scanner;

public class Interface {
    public static void main(String[] args) {
        Interface test = new Interface();
        test.start();
    }

    private void start() {
        while (true) {
            Scanner scan = new Scanner(System.in);
            String vendorName = scan.next();
            try {
                Class c = Class.forName(vendorName);
                Object o = c.newInstance();
                Vendor vendor = (Vendor) o;
                this.makeReservation(vendor);
            } catch (Exception ClassNotFoundException) {
                System.out.println("Vendor is not found");
            }
        }
    }

    public void makeReservation(Vendor vendor) {
        vendor.makeReservation();
    }
}
