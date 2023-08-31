/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    PatientDAO patientDAO = new PatientDAO(); // Main DAO objects.
    AdmissionDAO admissionDAO = new AdmissionDAO();

    public static void main(String[] args) { // This way methods in Main are not static.
        Main program = new Main();
        program.start(args);
    }

    public void start(String[] args) {
        this.readInput(args[0]);
    } // Reading argument as file.

    public void runInput(String[] parts, Writer writer) throws IOException { // Checking the first word of input file
                                                                             // and performing action.
        if (parts[0].equals("AddPatient")) {
            Patient temp = new Patient(Integer.parseInt(parts[1]), parts[2], parts[3], parts[4],
                    String.join(" ", Arrays.copyOfRange(parts, 5, parts.length)));// Creating new patient object.
            patientDAO.add(temp); // Using created patient object with patientDAO.add() method. This will write it
                                  // to the patient.txt file.
            writer.write("Patient " + parts[1] + " " + parts[2] + " added\n"); // Writing to output.txt
        } else if (parts[0].equals("RemovePatient")) {
            Patient removed = patientDAO.deleteByID(Integer.parseInt(parts[1])); // Using DAO removing patient object
                                                                                 // from patient.txt with given ID. This
                                                                                 // will return deleted object.
            writer.write("Patient " + String.valueOf(removed.getID()) + " " + removed.getName() + " removed\n"); // Using
                                                                                                                 // returned
                                                                                                                 // object
                                                                                                                 // printing
                                                                                                                 // output.
        } else if (parts[0].equals("CreateAdmission")) {
            Admission tempA = new Admission(Integer.parseInt(parts[1]), Integer.parseInt(parts[2])); // Creating
                                                                                                     // admission
                                                                                                     // object.
            admissionDAO.add(tempA); // Adding admission object to admission.txt with admissionDAO.
            writer.write("Admission " + String.valueOf(tempA.getID()) + " created\n"); // Writing to output.txt
        } else if (parts[0].equals("AddExamination")) {
            String[] passTo = Arrays.copyOfRange(parts, 2, parts.length); // Getting the examination part of line.
            Examination toAdd = admissionDAO.stringToExamination(String.join(" ", passTo)); // Passing it to examination
                                                                                            // generator object in DAO.
            admissionDAO.addExamination(Integer.parseInt(parts[1]), toAdd); // Using DAO adding generated examination to
                                                                            // according admission in admission.txt
            writer.write(parts[2] + " examination added to admission " + Integer.parseInt(parts[1]) + "\n"); // Writing
                                                                                                             // to
                                                                                                             // output.txt
        } else if (parts[0].equals("TotalCost")) {
            writer.write("TotalCost for admission " + String.valueOf(parts[1]) + "\n"); // Writing to output.txt
            writer.write(admissionDAO.getByID(Integer.parseInt(parts[1])).toOutputString()); // Getting the admission
                                                                                             // with according ID via
                                                                                             // admissionDAO from
                                                                                             // admission.txt and
                                                                                             // calling toOutputString
                                                                                             // method.
        } else if (parts[0].equals("ListPatients")) {
            writer.write("Patient List:\n");
            ArrayList<Patient> sorted = patientDAO.getALL(); // Getting array list of patients from patients.txt via
                                                             // patientDAO
            Collections.sort(sorted); // Sorting them by name.
            for (Patient patient : sorted) {
                writer.write(patient.toOutputString()); // Writing to output.txt by calling toOutputString method on
                                                        // each patient object.
            }
        }
    }

    public void readInput(String filename) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(filename))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) input.txt");
        }
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("output.txt")))); // Opening
                                                                                                                      // output.txt
                                                                                                                      // as
                                                                                                                      // writer.
            for (int i = 0; i < result.size(); i++) {
                if (result.get(i).equals("")) {
                    continue;
                } else if (result.get(i).equals(" ")) { // Ignoring invalid lines.
                    continue;
                }
                String[] parts = result.get(i).split(" ");
                this.runInput(parts, writer); // Passing on line and writer to runInput method.
            }
            writer.close();
        } catch (FileNotFoundException e) {
            System.err.println("Problem getting writer for output.txt");
            e.printStackTrace();
        } catch (IOException exception) {
            System.err.println("Problem writing to output.txt");
            exception.printStackTrace();
        }
    }
}
