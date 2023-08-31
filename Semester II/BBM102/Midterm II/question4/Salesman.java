package question4;

public abstract class Salesman {
    public abstract boolean makeReservation(String flight_number, String departureDate, String seat_type, String passengerID);
    public abstract boolean cancelReservation(String flight_number, String departureDate, String passengerID);
    public abstract boolean addPassenger(String name_of_Passenger, String gender, String email, String age, String passengerID);
    public abstract Passenger removePassenger(String passengerID);
}
