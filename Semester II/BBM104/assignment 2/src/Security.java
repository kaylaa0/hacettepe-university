/*
Hacettepe University
BBM 104 Assigment 2
b21726914
 */
public class Security extends Personnel{
    public Security(String name, String registration, String position, String start){
        super(name, registration ,position, start);
    }

    public double getHourOfWork(){
        double tempTotal = 0;
        for(Integer i = 0; i < workHours.size(); i++){
            if(workHours.get(i).compareTo(30)>=0){
                if(workHours.get(i).compareTo(54)<0){
                    tempTotal = tempTotal + workHours.get(i)*10;
                }else{
                    tempTotal = tempTotal + (54 * 10);
                }
                tempTotal = tempTotal + 90;
            }
        }
        return tempTotal;
    }

    @Override
    public void calculateSalary() {
        this.setSalary(this.getSalary() + this.getHourOfWork() + this.getSeverancePay());
    }
}
