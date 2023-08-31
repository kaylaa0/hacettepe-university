/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Chief extends FullTime {
    public Chief(String name, String registration, String position, String start) {
        super(name, registration, position, start);
    }

    @Override
    public void calculateSalary() {
        this.setSalary(this.getSalary() + getDayOfWork(125) + getSeverancePay() + getOverWorkSalary(8, 15));
    }
}
