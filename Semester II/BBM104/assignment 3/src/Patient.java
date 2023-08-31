/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Patient implements Comparable<Patient> {
    private int ID;
    private String name;
    private String surname;
    private String address;
    private String phone;

    public Patient(int ID, String name, String surname, String phone, String address) {
        this.ID = ID;
        this.name = name;
        this.surname = surname;
        this.address = address;
        this.phone = phone;
    }

    public int getID() {
        return ID;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() { // This one is for writing to patient.txt
        return String.valueOf(this.ID) + "\t" + this.name + " " + this.surname + "\t" + this.phone + "\tAddress: "
                + this.address + "\n";
    }

    public String toOutputString() { // This one is for writing to output.txt
        return String.valueOf(this.ID) + " " + this.name + " " + this.surname + " " + this.phone + " Address: "
                + this.address + "\n";
    }

    @Override
    public int compareTo(Patient o) { // Default comparing is done by the names of patients, we use different method
                                      // when we sort for patient.txt
        return this.getName().compareTo(o.getName());
    }
}
