/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

// The class name is Sports in order to match UML provided in assignment paper. However this class is a representation of a team.

public class Sports {

    private String clubName;
    private Integer matchesPlayed;
    private Integer matchesWon;
    private Integer matchesTied;
    private Integer matchesLost;
    private Integer setsWon;
    private Integer setsLost;
    private Integer goalDifference;
    private Integer points;

    public Sports(String name) {
        this.clubName = name;
        this.matchesWon = 0;
        this.matchesTied = 0;
        this.matchesLost = 0;
        this.matchesPlayed = this.matchesLost + this.matchesWon + this.matchesTied;
        this.setsWon = 0;
        this.setsLost = 0;
        this.goalDifference = this.setsWon - this.setsLost;
        this.points = 0;
        this.test();
    }

    public String getClubName() {
        return clubName;
    }

    public Integer getPoints() {
        return points;
    }

    public Integer getGoalDifference() {
        return goalDifference;
    }

    private void setSetsWon(Integer setsWon) {
        this.setsWon += setsWon;
    }

    private void setSetsLost(Integer setsLost) {
        this.setsLost += setsLost;
    }

    private void setMatchesLost() {
        this.matchesLost += 1;
    }

    private void setMatchesWon() {
        this.matchesWon += 1;
    }

    private void setMatchesTied() {
        this.matchesTied += 1;
    }

    private void setMatchesPlayed() {
        this.matchesPlayed = this.matchesLost + this.matchesWon + this.matchesTied;
    }

    private void setGoalDifference() {
        this.goalDifference = this.setsWon - this.setsLost;
    }

    protected void setPoints(Integer points) {
        this.points += points;
    }

    public void result(Integer direction, String result) {
        String[] parts = result.split(":");
        if (direction == -1) {
            this.result(Integer.parseInt(parts[0]), Integer.parseInt(parts[1]));
        } else if (direction == 1) {
            this.result(Integer.parseInt(parts[1]), Integer.parseInt(parts[0]));
        }
    }

    public void test() {
        System.out.println("1");
    }

    public void result(Integer won, Integer lost) {
        this.setSetsWon(won);
        this.setSetsLost(lost);
        if (won > lost) {
            setMatchesWon();
        } else if (lost > won) {
            setMatchesLost();
        } else if (lost == won) {
            setMatchesTied();
        }
        this.setMatchesPlayed();
        this.setGoalDifference();
    }

    @Override
    public String toString() {
        return clubName + "\t" +
                matchesPlayed + "\t" +
                matchesWon + "\t" +
                matchesTied + "\t" +
                matchesLost + "\t" +
                setsWon + ":" +
                setsLost + "\t" +
                points;
    }

}
