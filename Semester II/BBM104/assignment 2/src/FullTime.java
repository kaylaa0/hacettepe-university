/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class FullTime extends Employee {
    public FullTime(String name, String registration, String position, String start) {
        super(name, registration, position, start);
    }

    // getDayOfWork method shared both by Chief and Worker
    public double getDayOfWork(Integer perDay) {
        return perDay * 20;
    }
}
