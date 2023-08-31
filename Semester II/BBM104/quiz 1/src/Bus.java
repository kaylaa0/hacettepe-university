/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.ArrayList;

public class Bus {
    public String plate;
    public int seatCount;
    public Seat[] seats;

    public Bus(String plate, int seatCount) {
        this.plate = plate;
        this.seatCount = seatCount;
        seats = new Seat[seatCount];
        for (int i = 0; i < seatCount; i++) {
            Seat dummy = new Seat(i + 1);
            this.seats[i] = dummy;
        }
    }

    public String getPlate() {
        return plate;
    }

    public void setPlate(String plate) {
        this.plate = plate;
    }

    public int getSeatCount() {
        return seatCount;
    }

    public void setSeatCount(int seatCount) {
        this.seatCount = seatCount;
    }

    public Seat[] getSeats() {
        return seats;
    }

    public void setSeats(Seat[] seats) {
        this.seats = seats;
    }

    public void bookSeat(Passenger p, int seatID) {
        Seat dummy = new Seat(seatID, p);
        this.seats[seatID - 1] = dummy;
        this.seats[seatID - 1].setStatus(true);
    }

    public void printAllPassengers() {
        for (int i = 0; i < this.seatCount; i++) {
            if (seats[i].status) {
                seats[i].display();
            }
        }
    }

    public void printAvailableSeatIDs() {
        for (int i = 0; i < this.seatCount; i++) {
            if (seats[i].status == false) {
                seats[i].display();
            }
        }
    }

    public int search(String name, String surname) {
        for (int i = 0; i < this.seatCount; i++) {
            if (seats[i].getStatus() == true) {
                if (seats[i].passenger.name.equals(name) && seats[i].passenger.surname.equals(surname)) {
                    System.out.println("Passenger found");
                    return seats[i].seatID;
                }
            }

        }
        return 0;
    }

}
