/*
Hacettepe University
BBM 104 Assigment 2
b21726914
 */
public class Worker extends FullTime {
    public Worker(String name, String registration, String position, String start){
        super(name, registration, position, start);
    }

    @Override
    public void calculateSalary() {
        this.setSalary(this.getSalary() + getDayOfWork(105) + getSeverancePay() + getOverWorkSalary(10, 11));
    }
}
