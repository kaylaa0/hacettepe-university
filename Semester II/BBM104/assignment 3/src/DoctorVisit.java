/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class DoctorVisit extends Decorator {
    public DoctorVisit() {
        super();
    }

    public DoctorVisit(Decorator examination) {
        super(examination);
    }

    @Override
    public int cost() {
        return super.cost() + 15;
    }
}
