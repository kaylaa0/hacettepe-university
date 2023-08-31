/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Assistants extends Academician {
    public Assistants(String name, String registration, String position, String start) {
        super(name, registration, position, start);
    }

    @Override
    public double getSSBenefits() {
        return (this.getBaseSalary() * 105) / 100;
    }
}
