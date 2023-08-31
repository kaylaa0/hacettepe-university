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

public class AdmissionDAO implements DataAccessObject {
    @Override
    public Admission getByID(int ID) {
        ArrayList<Admission> temp = this.getALL(); // Reading admission txt and getting all the objects.
        for (Admission admission : temp) { // Searching for ID match.
            if (admission.getID() == ID) {
                return admission; // Returning object when found match.
            }
        }
        return null; // Returning null when object is not found.
    }

    @Override
    public Admission deleteByID(int ID) {
        ArrayList<Admission> newArray = this.getALL(); // Reading admission txt and getting all the objects.
        for (Admission admission : newArray) { // Searching for ID match.
            if (admission.getID() == ID) {
                newArray.remove(admission); // Removing object from arraylist.
                this.saveArray(newArray); // Saving new arraylist to admission.txt
                return admission; // Returning deleted item.
            }
        }
        return null;
    }

    public Admission deleteByPatientID(int ID) {
        ArrayList<Admission> newArray = this.getALL(); // Reading admission txt and getting all the objects.
        for (Admission admission : newArray) { // Searching for patient ID match.
            if (admission.getPatientID() == ID) { // If patient ID is matched removing object and saving to
                                                  // admission.txt
                newArray.remove(admission);
                this.saveArray(newArray); // This method used when we delete a patient with patient ID we call this one
                                          // to delete according admissions as well.
                return admission;
            }
        }
        return null;
    }

    private void saveArray(ArrayList<Admission> newArray) {
        try {
            Collections.sort(newArray, new Comparator<Admission>() { // Sorting array according to admission IDS.
                @Override
                public int compare(Admission a1, Admission a2) {
                    Integer v1 = a1.getID();
                    Integer v2 = a2.getID();
                    return v1.compareTo(v2);
                }
            });
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("admission.txt"))));
            for (Admission admission : newArray) {
                writer.write(((Admission) admission).toString()); // Writing all objects in array to admission.txt
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to admission.txt");
            e.printStackTrace();
        }
    }

    @Override
    public void add(Object admission) {
        ArrayList<Admission> temp = this.getALL(); // Getting object with reading admission.txt
        temp.add((Admission) admission); // Adding new object.
        this.saveArray(temp); // Saving to the file.
    }

    public void addExamination(int ID, Examination examination) {
        ArrayList<Admission> newArray = this.getALL(); // Reading admission txt and getting all the objects.
        for (Admission admission : newArray) {
            if (admission.getID() == ID) { // Searching for ID match.
                admission.addExamination(examination); // Adding examinations to matched admission.
                this.saveArray(newArray); // Saving to the file.
            }
        }
    }

    @Override
    public ArrayList<Admission> getALL() {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get("admission.txt"))) { // Reading admission.txt
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) admission.txt");
        }
        ArrayList<Admission> admissions = new ArrayList<Admission>(); // Final admission list that will be returned.
        ArrayList<Object> tempObject = new ArrayList<Object>(); // Currently read admission ID and according patient ID
                                                                // is stored in here.
        ArrayList<Examination> tempList = new ArrayList<Examination>(); // Currently read examinations stored in here.
        String[] parts0 = result.get(0).split("\t"); // First line is read separately and done according methods.
        tempObject.add(parts0[0]);
        tempObject.add(parts0[1]);
        for (int i = 1; i < result.size(); i++) {
            if (result.get(i).equals("")) {
                continue;
            } else if (result.get(i).equals(" ")) { // Checking if line is valid.
                continue;
            }
            String[] parts = result.get(i).split("\t");
            if (parts[0].equals("Inpatient")) {
                tempList.add(this.stringToExamination(parts)); // Creating examination objects and adding them to
                                                               // current list.
            } else if (parts[0].equals("Outpatient")) {
                tempList.add(this.stringToExamination(parts));
            } else { // If we reach a line with new admission command first we create and add
                     // currently stored admission data to arraylist.
                admissions.add(new Admission(Integer.parseInt(tempObject.get(0).toString()),
                        Integer.parseInt(tempObject.get(1).toString()), this.examinationsFinalizer(tempList)));
                tempObject = new ArrayList<Object>(); // Empty out current stored temp data and continue reading.
                tempObject.add(parts[0]);
                tempObject.add(parts[1]);
                tempList = new ArrayList<Examination>();
            }
        }
        // At the end of the file adding currently stored (last) admission object to
        // arraylist.
        admissions.add(new Admission(Integer.parseInt(tempObject.get(0).toString()),
                Integer.parseInt(tempObject.get(1).toString()), this.examinationsFinalizer(tempList)));
        return admissions; // Returning arraylist.
    }

    public Examination[] examinationsFinalizer(ArrayList<Examination> theArray) { // Converting arraylist examination to
                                                                                  // array examination.
        Examination[] result = new Examination[theArray.size()];
        for (int i = 0; i < theArray.size(); i++) {
            result[i] = theArray.get(i);
        }
        return result;
    }

    public Examination stringToExamination(String[] part) { // Reading string and generating according examinations
                                                            // file.
        Examination temp;
        String[] parts = part[1].split(" ");
        Decorator first = this.stringToDecorator(parts[0]); // Generating first decorator.
        for (int i = 1; i < parts.length; i++) { // According to next word in string, putting that decorator inside
                                                 // another decorators generator and getting new decorator. There is no
                                                 // limit.
            if (parts[i].equals("tests")) {
                first = new Tests(first);
            } else if (parts[i].equals("doctorvisit")) {
                first = new DoctorVisit(first);
            } else if (parts[i].equals("imaging")) {
                first = new Imaging(first);
            } else if (parts[i].equals("measurements")) {
                first = new Measurements(first);
            }
        }
        if (part[0].equals("Inpatient")) { // Lastly generating examination tpye object and putting decorator via
                                           // addoperation().
            temp = new Inpatient(); // We could generate this part by putting inside generator as well but in order
                                    // to match assigment paper I added this method too.
            temp.addoperation(first);
            return temp;
        } else if (part[0].equals("Outpatient")) {
            temp = new Outpatient();
            temp.addoperation(first);
            return temp; // Returning generated object.
        }
        return null;
    }

    public Examination stringToExamination(String string) { // The difference of this algorithm from the one above is
                                                            // the input it takes. Above algorithm is used when reading
                                                            // patient.txt
        Examination temp; // This one used when reading input.txt
        String[] parts = string.split(" ");
        Decorator first = this.stringToDecorator(parts[1]);
        for (int i = 2; i < parts.length; i++) {
            if (parts[i].equals("tests")) {
                first = new Tests(first);
            } else if (parts[i].equals("doctorvisit")) {
                first = new DoctorVisit(first);
            } else if (parts[i].equals("imaging")) {
                first = new Imaging(first);
            } else if (parts[i].equals("measurements")) {
                first = new Measurements(first);
            }
        }
        if (parts[0].equals("Inpatient")) {
            temp = new Inpatient();
            temp.addoperation(first);
            return temp;
        } else if (parts[0].equals("Outpatient")) {
            temp = new Outpatient();
            temp.addoperation(first);
            return temp;
        }
        return null;
    }

    private Decorator stringToDecorator(String name) { // Getting first decorator object that will be put inner most
                                                       // part of generation with this method.
        if (name.equals("tests")) {
            return new Tests();
        } else if (name.equals("doctorvisit")) {
            return new DoctorVisit();
        } else if (name.equals("imaging")) {
            return new Imaging();
        } else if (name.equals("measurements")) {
            return new Measurements();
        }
        return null;
    }
}
