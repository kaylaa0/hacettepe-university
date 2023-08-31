/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Volleyball extends Sports {

    public Volleyball(String name) {
        super(name);
    }

    @Override
    public void result(Integer won, Integer lost) {
        super.result(won, lost);
        if (won.equals(3)) {
            if (lost.equals(2)) {
                setPoints(2);
            } else {
                setPoints(3);
            }
        } else if (won.equals(2)) {
            setPoints(1);
        }
    }
}
