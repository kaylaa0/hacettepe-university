/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    public ArrayList<Contact> arrayList = new ArrayList<Contact>();
    public HashSet<Contact> hashSet = new HashSet<Contact>();
    public TreeSet<Contact> treeSet = new TreeSet<Contact>();
    public TreeSet<Contact> sortedTreeSet = new TreeSet<Contact>(new LastNameComparator());
    public HashMap<String, Contact> hashMap = new HashMap<String, Contact>();

    public static void main(String[] args) {
        Main program = new Main();
        program.start(args);
    }

    public void start(String[] args) {
        this.readFile(args[0]);
        this.printData(arrayList, "contactsArrayList.txt");
        Collections.sort(arrayList, new LastNameComparator());
        this.printData(arrayList, "contactsArrayListOrderByLastName.txt");
        this.printData(hashSet, "contactsHashSet.txt");
        this.printData(treeSet, "contactsTreeSet.txt");
        this.printData(sortedTreeSet, "contactsTreeSetOrderByLastName.txt");
        this.printData(hashMap.values(), "contactsHashMap.txt");
    }

    public void readFile(String filename) {
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(filename))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) input.txt");
        }
        for (int i = 0; i < result.size(); i++) {
            if (result.get(i).equals("")) {
                continue;
            } else if (result.get(i).equals(" ")) { // Ignoring invalid lines.
                continue;
            } else if (result.get(i).equals("\n")) {
                continue;
            }
            String[] parts = result.get(i).split(" ");
            Contact newContact = new Contact(parts[0], parts[1], parts[2], parts[3]);
            this.arrayList.add(newContact);
            this.hashSet.add(newContact);
            this.treeSet.add(newContact);
            this.hashMap.put(newContact.getPhoneNumber(), newContact);
            this.sortedTreeSet.add(newContact);
        }
    }

    public void printData(Collection<Contact> set, String filename) {
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(filename))));
            for (Contact contact : set) {
                writer.write(contact.toString() + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to " + filename);
            e.printStackTrace();
        }
    }
}
