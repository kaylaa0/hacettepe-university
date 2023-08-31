/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Admission {
    private int ID;
    private int patientID;
    private Examination[] examinations;

    public Admission(int ID, int patientID) { // This is used when read a line from input.txt and generate admission
                                              // object to write in admission.txt
        this.ID = ID;
        this.patientID = patientID;
        this.examinations = new Examination[0];
    }

    public Admission(int ID, int patientID, Examination[] examinations) { // This is used when reading admission.txt
                                                                          // with admissionDAO
        this.ID = ID;
        this.patientID = patientID;
        this.examinations = examinations;
    }

    public void addExamination(Examination examination) { // This is used when we read input.txt and add examination to
                                                          // according admission.
        Examination[] temp = this.examinations; // Copying current examinations.
        this.examinations = new Examination[this.examinations.length + 1]; // Generating new examinations array with one
                                                                           // more length than current one.
        for (int examin = 0; examin < temp.length; examin++) { // Copying previous examinations to current one.
            this.examinations[examin] = temp[examin];
        }
        this.examinations[this.examinations.length - 1] = examination; // Adding the examination to last place.
    }

    public int getID() {
        return ID;
    }

    public int getPatientID() {
        return patientID;
    }

    @Override
    public String toString() { // This method used when printing to admission.txt
        StringBuilder build = new StringBuilder(String.valueOf(this.ID) + "\t" + String.valueOf(this.patientID) + "\n");
        for (Examination e : this.examinations) {
            build.append(e.printoperations()).append("\n");
        }
        return build.toString();
    }

    public String toOutputString() { // This method used when printing to output.txt
        StringBuilder build = new StringBuilder();
        int total = 0;
        for (Examination e : this.examinations) {
            build.append("\t").append(e.printoperations().split("\t")[0]).append(" ")
                    .append(e.printoperations().split("\t")[1]).append(" ").append(String.valueOf(e.cost())).append("$")
                    .append("\n");
            total += e.cost();
        }
        build.append("\t").append("Total: ").append(String.valueOf(total)).append("$").append("\n");
        return build.toString();
    }
}
