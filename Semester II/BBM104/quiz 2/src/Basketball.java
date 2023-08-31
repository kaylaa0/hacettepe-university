/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Basketball extends Sports {

    public Basketball(String name) {
        super(name);
    }

    @Override
    public void result(Integer won, Integer lost) {
        super.result(won, lost);
        if (won > lost) {
            setPoints(2);
        } else if (won < lost) {
            setPoints(1);
        }
    }
}
