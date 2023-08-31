/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Employee {
    // This class is created according to assigment paper.
    private String employeeCode;
    private String NRIC;
    private int phone;

    public Employee(String employeeCode, String NRIC, int phone) {
        this.phone = phone;
        this.NRIC = NRIC;
        this.employeeCode = employeeCode;
    }

    public int getPhone() {
        return phone;
    }

    public String getEmployeeCode() {
        return employeeCode;
    }

    public String getNRIC() {
        return NRIC;
    }

    // This method is overridden to ease debugging.
    @Override
    public String toString() {
        return employeeCode + " " + NRIC + " " + String.valueOf(phone);
    }
}
