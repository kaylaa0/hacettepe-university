/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Faculty extends Academician {
    public Faculty(String name, String registration, String position, String start) {
        super(name, registration, position, start);
    }

    @Override
    public double getSSBenefits() {
        return (this.getBaseSalary() * 135) / 100;
    }

    public double getAddCourseFee() {
        return super.getOverWorkSalary(8, 20);
    }

    @Override
    public void calculateSalary() {
        // At Personnel class (super) we already have getSeverancePay() +
        // getBaseSalary() + getSSBenefits()
        super.calculateSalary();
        setSalary(this.getSalary() + this.getAddCourseFee());
    }
}
