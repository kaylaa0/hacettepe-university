/*
Hacettepe University
BBM 104 Assigment 2
b21726914
 */
import java.util.ArrayList;
import java.util.List;

public class Personnel {
    public String name;
    public String registration;
    public String position;
    public String startYear;
    public List<Integer> workHours = new ArrayList<Integer>();
    public double salary;

    public Personnel(String name, String registration, String position, String start){
        this.name = name;
        this.registration = registration;
        this.position = position;
        this.startYear = start;
        this.workHours.add(0);
        this.workHours.add(0);
        this.workHours.add(0);
        this.workHours.add(0);
        this.salary = 0;
    }

    public void setWorkHours(Integer week1, Integer week2, Integer week3, Integer week4){
        this.workHours.set(0,week1);
        this.workHours.set(1,week2);
        this.workHours.set(2,week3);
        this.workHours.set(3,week4);
    }

    public double getBaseSalary(){
        return 2600;
    }

    // Should override at faculty member for %135, research assistant for %105 officers are already set.
    public double getSSBenefits() {
        return (this.getBaseSalary()*65)/100;
    }

    public double getSeverancePay(){
        return (2020 - Integer.parseInt(this.getStartYear()))*20*0.8;
    }

    public void calculateSalary(){
        setSalary(this.getSalary() + this.getSeverancePay() + this.getBaseSalary() + this.getSSBenefits());
    }

    public double getOverWorkSalary(Integer maxHours, Integer perHour){
        double tempTotal = 0;
        for(Integer i = 0; i < workHours.size(); i++){
            if(workHours.get(i).compareTo(40)>=0){
                if(workHours.get(i).compareTo(40+maxHours)<0){
                    tempTotal = tempTotal + (workHours.get(i)-40)*perHour;
                }else{
                    tempTotal = tempTotal + (maxHours * perHour);
                }
            }
        }
        return tempTotal;
    }

    public void setSalary(double salary){
        this.salary = salary;
    }

    public double getSalary() {
        return salary;
    }

    public String getName() {
        return name;
    }

    public String getRegistration() {
        return registration;
    }

    public String getPosition() {
        return position;
    }

    public String getStartYear() {
        return startYear;
    }

    @Override
    public String toString() {
        return  "Name : "+ this.getName().split(" ")[0] + "\n"+
                "Surname : "+ this.getName().split(" ")[1] + "\n"+
                "Registration Number : "+ this.getRegistration() + "\n"+
                "Position : "+ this.getPosition() + "\n"+
                "Year of Start : "+ this.getStartYear() + "\n"+
                "Total Salary : "+ this.getSalary() + "0 TL";
    }



}
