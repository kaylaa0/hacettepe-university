/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Inpatient implements Examination {
    private Decorator examination;

    public Inpatient() {
        super(); // I am not sure if this method is needed but I add it anyway. This way
                 // Inpatient and Outpatient files are also can be used as decorators.
    }

    public Inpatient(Decorator a) {
        this.examination = a; // I am not sure if this method is needed but I add it anyway.
    }

    @Override
    public void addoperation(Decorator a) {
        this.examination = a;
    }

    @Override
    public String printoperations() {
        StringBuilder temp = new StringBuilder(this.getClass().getSimpleName());
        return temp.append("\t").append(this.examination.printoperations()).toString();
    }

    @Override
    public int cost() {
        return this.examination.cost() + 10;
    }
}
