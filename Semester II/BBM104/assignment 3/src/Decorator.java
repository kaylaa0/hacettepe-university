/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Decorator implements Examination {
    private Decorator examination;

    public Decorator() { // Instead of general decorator usage, which is implemented, I also added
                         // addoperation etc. to match assigment paper. Currently decorator design is
                         // valid and also all things from assigment paper are here.
        this.examination = null;
    }

    public Decorator(Decorator examination) {
        this.examination = examination;
    }

    @Override
    public void addoperation(Decorator a) {
        this.examination = a;
    }

    @Override
    public String printoperations() {
        if (this.examination != null) {
            StringBuilder temp = new StringBuilder(this.examination.printoperations());
            return temp.append(" ").append(this.getClass().getSimpleName()).toString().toLowerCase();
        } else {
            return this.getClass().getSimpleName().toLowerCase();
        }

    }

    @Override
    public int cost() {
        if (this.examination != null) {
            return this.examination.cost();
        }
        return 0;
    }
}
