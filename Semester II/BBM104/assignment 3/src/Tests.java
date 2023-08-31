/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Tests extends Decorator {
    public Tests() {
        super();
    }

    public Tests(Decorator examination) {
        super(examination);
    }

    @Override
    public int cost() {
        return super.cost() + 7;
    }
}
