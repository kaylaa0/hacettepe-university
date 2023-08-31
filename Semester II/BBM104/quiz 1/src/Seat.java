/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Seat {
    public int seatID;
    public boolean status;
    public Passenger passenger;

    public Seat(int seatID, Passenger p) {
        this.seatID = seatID;
        this.passenger = p;
    }

    public Seat(int seatID) {
        this.seatID = seatID;
    }

    public int getSeatID() {
        return seatID;
    }

    public void setSeatID(int seatID) {
        this.seatID = seatID;
    }

    public Passenger getPassenger() {
        return passenger;
    }

    public void setPassenger(Passenger passenger) {
        this.passenger = passenger;
    }

    public boolean getStatus() {
        return status;
    }

    public void setStatus(boolean status) {
        this.status = status;
    }

    public void display() {
        if (this.status) {
            System.out.println("Seat: " + seatID + " Status: Reserved");
            this.passenger.display();
        } else {
            System.out.println("Seat: " + seatID + " Status: Available");
        }

    }

}
