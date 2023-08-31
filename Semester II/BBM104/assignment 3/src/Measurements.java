/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Measurements extends Decorator {
    public Measurements() {
        super();
    }

    public Measurements(Decorator examination) {
        super(examination);
    }

    @Override
    public int cost() {
        return super.cost() + 5;
    }
}
