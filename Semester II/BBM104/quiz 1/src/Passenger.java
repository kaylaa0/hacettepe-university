/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Passenger {
    public String name;
    public String surname;
    public String gender;
    public Phone phone;

    public Passenger(String name, String surname, String gender, Phone phone) {
        this.name = name;
        this.surname = surname;
        this.gender = gender;
        this.phone = phone;
    }

    public String getName() {
        return name;
    }

    public String getSurname() {
        return surname;
    }

    public String getGender() {
        return gender;
    }

    public Phone getPhone() {
        return phone;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public void setPhone(Phone phone) {
        this.phone = phone;
    }

    public void display() {
        System.out.println(name + " " + surname + " (" + gender + ")");
        this.phone.display();
    }
}
