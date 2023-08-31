/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class PatientDAO implements DataAccessObject {
    private AdmissionDAO admissionDAO = new AdmissionDAO(); // There is an admissionDAO here because we remove admission
                                                            // from admission.txt when we remove a patient from
                                                            // patient.txt

    @Override
    public Patient getByID(int ID) { // Everything in this file same as admissionDAO. I inform you to check
                                     // admissionDAO first. I will add comments only to differences here.
        ArrayList<Patient> temp = this.getALL(); // Main difference is everything is performed to patient.txt with
                                                 // patient objects instead.
        for (Patient patient : temp) {
            if (patient.getID() == ID) {
                return patient;
            }
        }
        return null;
    }

    @Override
    public Patient deleteByID(int ID) {
        ArrayList<Patient> newArray = this.getALL();
        for (Patient patient : newArray) {
            if (patient.getID() == ID) {
                newArray.remove(patient);
                this.saveArray(newArray);
                Admission trash = this.admissionDAO.deleteByPatientID(ID); // Also calling deleting by patient ID method
                                                                           // with admissionDAO
                return patient;
            }
        }
        return null;
    }

    private void saveArray(ArrayList<Patient> newArray) {
        try {
            Collections.sort(newArray, new Comparator<Patient>() {
                @Override
                public int compare(Patient a1, Patient a2) {
                    Integer v1 = a1.getID();
                    Integer v2 = a2.getID();
                    return v1.compareTo(v2);
                }
            });
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("patient.txt"))));
            for (Patient patient : newArray) {
                writer.write(((Patient) patient).toString());
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to patient.txt");
            e.printStackTrace();
        }
    }

    @Override
    public void add(Object patient) {
        ArrayList<Patient> temp = this.getALL();
        temp.add((Patient) patient);
        this.saveArray(temp);
    }

    @Override
    public ArrayList<Patient> getALL() {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get("patient.txt"))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) patient.txt");
        }
        ArrayList<Patient> patients = new ArrayList<Patient>();
        for (int i = 0; i < result.size(); i++) {
            if (result.get(i).equals("")) {
                continue;
            } else if (result.get(i).equals(" ")) {
                continue;
            }
            String[] parts = result.get(i).split("\t"); // According to every line a new patient object created and
                                                        // added to arraylist.
            patients.add(new Patient(Integer.parseInt(parts[0]), parts[1].split(" ")[0], parts[1].split(" ")[1],
                    parts[2], parts[3].replace("Address: ", "")));
        }
        return patients; // Arraylist returned.
    }
}
