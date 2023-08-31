
public class Program {
    public static void main(String[] args) {
        try {
            Score bill = new Score("Bill Gates", 84);
            Score sevil = new Score("Sevil Sen", 91);
            // Determine how bob compares to mary.
            // It throws exception if different types of objects are to be compared
            int comparison = bill.compareTo(sevil);

            // Print out the results of the comparison.
            System.out.print("Bill's test score was ");
            if (comparison == -1)
                System.out.print("less than ");
            else if (comparison == 0)
                System.out.print("equal to ");
            else
                System.out.print("greater than ");

            System.out.println("Sevil's test score.");
        } catch (Exception ex) {
            System.out.println(ex.toString());
        }
    }
}

class Score implements Comparable<Object> {
    private String name;
    private Integer score;

    public Score(String name, int score) {
        this.name = name;
        this.score = score;
    }

    @Override
    public int compareTo(Object o) throws ClassCastException {
        if (o instanceof Score) {
            return this.score.compareTo(((Score) o).score);
        } else {
            throw new ClassCastException();
        }

    }
}
