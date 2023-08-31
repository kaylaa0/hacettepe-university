/*
Hacettepe University
BBM 104 Assigment 2
b21726914
 */
public class PartTime extends Employee {
    public PartTime(String name, String registration, String position, String start){
        super(name, registration ,position, start);
    }

    public double getHourOfWork(){
        double tempTotal = 0;
        for(Integer i = 0; i < workHours.size(); i++){
            if(workHours.get(i).compareTo(10)>=0){
                if(workHours.get(i).compareTo(20)<0){
                    tempTotal = tempTotal + workHours.get(i)*18;
                }else{
                    tempTotal = tempTotal + (20 * 18);
                }
            }
        }
        return tempTotal;
    }

    @Override
    public void calculateSalary() {
        this.setSalary(this.getSalary() + getHourOfWork() + getSeverancePay());
    }
}
