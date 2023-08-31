/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Handball extends Sports {

    public Handball(String name) {
        super(name);
    }

    @Override
    public void result(Integer won, Integer lost) {
        super.result(won, lost);
        if (won > lost) {
            setPoints(2);
        } else if (won == lost) {
            setPoints(1);
        }
    }

    @Override
    public void test() {
        System.out.println(2);
    }
}
