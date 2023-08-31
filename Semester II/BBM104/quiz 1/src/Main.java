/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.InputMismatchException;
import java.util.Scanner;

public class Main {

    public static boolean exit;

    public static void display() {
        System.out.print("1-Book a seat\n" +
                "2-Print all passengers\n" +
                "3-Print all seats\n" +
                "4-Search passenger\n" +
                "5-Exit\n" +
                "Enter your choose: ");
    }

    public static void book(Bus bus) {
        Scanner scan = new Scanner(System.in);
        int id = 0;
        String name;
        String surname;
        String gender;
        String countryCode;
        int code = -1;
        int number = -1;
        String type;
        while (!(id < bus.seatCount && 0 < id)) {
            try {
                System.out.print("Enter seat id: ");
                id = scan.nextInt();
            } catch (InputMismatchException e) {
                scan.nextLine();
            } finally {
                if (id < bus.seatCount && 0 < id) {
                    scan.nextLine();
                    break;
                } else {
                    System.out.println("Invalid seat id.");
                }
            }
        }
        System.out.print("Enter name: ");
        name = scan.nextLine();
        System.out.print("Enter surname: ");
        surname = scan.nextLine();
        System.out.print("Enter gender: ");
        gender = scan.nextLine();
        System.out.print("Enter country code: ");
        countryCode = scan.nextLine();
        while (!(-1 < code && code <= 9999)) {
            try {
                System.out.print("Enter code: ");
                code = scan.nextInt();
            } catch (InputMismatchException e) {
                scan.nextLine();
            } finally {
                if (-1 < code && code <= 9999) {
                    scan.nextLine();
                    break;
                } else {
                    System.out.println("Invalid operator code.");
                }
            }
        }
        while (!(1000000 < number && number <= 9999999)) {
            try {
                System.out.print("Enter number: ");
                number = scan.nextInt();
            } catch (InputMismatchException e) {
                scan.nextLine();
            } finally {
                if (1000000 < number && number <= 9999999) {
                    scan.nextLine();
                    break;
                } else {
                    System.out.println("Invalid phone number.");
                }
            }
        }
        System.out.print("Enter type: ");
        type = scan.next();
        if (bus.seats[id - 1].status == true) {
            System.out.println("The seat has already reserved.");
        } else {
            Phone phone;
            if (countryCode.equals("")) {
                phone = new Phone(code, number, type);
            } else {
                phone = new Phone(countryCode, code, number, type);
            }
            Passenger passenger = new Passenger(name, surname, gender, phone);
            bus.bookSeat(passenger, id);
        }
    }

    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        String input;
        Bus bus1 = new Bus("06 HUBM 06", 42);
        while (!exit) {
            Main.display();
            input = scan.nextLine();
            switch (input) {
                case "1":
                    Main.book(bus1);
                    break;
                case "2":
                    bus1.printAllPassengers();
                    break;
                case "3":
                    bus1.printAvailableSeatIDs();
                    break;
                case "4":

                    String name;
                    System.out.print("Enter name: ");
                    name = scan.nextLine();
                    System.out.print("Enter surname: ");
                    String surname = scan.nextLine();
                    int id = bus1.search(name, surname);
                    if (id > 0) {
                        bus1.seats[id - 1].passenger.display();
                    } else {
                        System.out.println("Passenger not found");
                    }
                    break;
                case "5":
                    exit = true;
                    break;
            }
        }
    }
}
