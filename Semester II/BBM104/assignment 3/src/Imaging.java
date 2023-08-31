/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Imaging extends Decorator {
    public Imaging() {
        super();
    }

    public Imaging(Decorator examination) {
        super(examination);
    }

    @Override
    public int cost() {
        return super.cost() + 10;
    }
}
