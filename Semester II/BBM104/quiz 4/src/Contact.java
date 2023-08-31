/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Contact implements Comparable<Contact> {
    private String socialSecurityNumber;
    private String firstName;
    private String lastName;
    private String phoneNumber;

    public Contact(String phoneNumber, String firstName, String lastName, String socialSecurityNumber) {
        this.socialSecurityNumber = socialSecurityNumber;
        this.firstName = firstName;
        this.lastName = lastName;
        this.phoneNumber = phoneNumber;
    }

    public String getLastName() {
        return lastName;
    }

    public String getPhoneNumber() {
        return phoneNumber;
    }

    @Override
    public int compareTo(Contact o) {
        return this.getPhoneNumber().compareTo(o.getPhoneNumber());
    }

    @Override
    public String toString() {
        return this.phoneNumber + " " + this.firstName + " " + this.lastName + " " + this.socialSecurityNumber;
    }
}
