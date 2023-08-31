/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Hockey extends Sports {

    public Hockey(String name) {
        super(name);
    }

    @Override
    public void result(Integer won, Integer lost) {
        super.result(won, lost);
        if (won > lost) {
            setPoints(3);
        } else if (won == lost) {
            setPoints(1);
        }
    }
}
