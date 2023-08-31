/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Officer extends Personnel {
    public Officer(String name, String registration, String position, String start) {
        super(name, registration, position, start);
    }

    @Override
    public void calculateSalary() {
        super.calculateSalary();
        setSalary(this.getSalary() + this.getOverWorkSalary(10, 20));
    }
}
