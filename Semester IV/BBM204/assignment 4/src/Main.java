/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    // Change these as your wish, if you encounter a problem make sure you have
    // correct settings.
    public String inputPath = ""; // In eclipse inputs are read from parent directory, in order to read them from
                                  // src set to ./src/. It is really hard to understand what TA wants, please
                                  // don't minus 10 points from me.
    public String outputPath = ""; // I use JetBrains IntelliJ, if you encounter problems please change this to
                                   // empty and run program using javac.
    public int n_gram = 3; // n_gram value.
    public int FEATURE_SIZE = 5000; // FEATURE_SIZE value.
    public String inputFileEncoding = "Cp1252"; // This is very important Cp1252 worked for me with output you shared
                                                // via piazza on win10 IntelliJ. Default encodings don't work. Please
                                                // know your file encoding and specify correctly.

    // Variables of Main.
    public List<Node> strongNodes; // This is used to determine significant n grams.
    public List<String> legitimateTestFile; // Loaded test file.
    public List<String> phishingTestFile; // Loaded test file.
    public TST learnedTST; // Final TST for getting weights.

    // Variables for test results.
    private int True_Positive = 0; // (TP): indicates the number of cases where actual URL is phishing and
                                   // predicted is also true saying it is phishing
    private int False_Negative = 0; // (FN): indicates the number of cases where actual URL is phishing but the
                                    // prediction was wrongly made – saying it is legitimate.
    private int True_Negative = 0; // (TN): indicates the number of cases where actual URL is legitimate and the
                                   // prediction is also true saying it is legitimate.
    private int False_Positive = 0; // (FP): indicates the number of cases where actual URL is legitimate but the
                                    // prediction was wrongly made – saying it is phishing.
    private int Unpredictable_Phishing = 0; // (UP): indicates the number of cases where actual URL is phishing but
                                            // computed total score is zero.
    private int Unpredictable_Legitimate = 0; // (UL): indicates the number of cases where actual URL is legitimate but
                                              // computed total score is zero.

    public void start() {
        System.out.println("n-gram based phishing detection via TST\n");
        // Node ghostHead = new Node(getMidOf()); // This method adds n "O" ngram string
        // first to the head, in order to somewhat balance the tree.
        // newTST.addNode(ghostHead); This method is no longer used because exact output
        // is more important.

        // Loading legitimate training file.
        List<String> legitTrain = readFile("legitimatetrain.txt");
        System.out.println(
                "Legitimate training file has been loaded with [" + String.valueOf(legitTrain.size()) + "] instances");

        // Loading legitimate test file.
        this.legitimateTestFile = readFile("legitimatetest.txt");
        System.out.println("Legitimate test file has been loaded with ["
                + String.valueOf(this.legitimateTestFile.size()) + "] instances");

        // Loading phishing training file.
        List<String> phishingTrain = readFile("phishingtrain.txt");
        System.out.println(
                "Phishing training file has been loaded with [" + String.valueOf(phishingTrain.size()) + "] instances");

        // Loading phishing test file.
        this.phishingTestFile = readFile("phishingtest.txt");
        System.out.println("Phishing test file has been loaded with [" + String.valueOf(this.phishingTestFile.size())
                + "] instances");

        TST newTST = new TST(this.n_gram, this.FEATURE_SIZE, this.outputPath); // A new TST is created with settings.

        newTST.evaluateFile(legitTrain, 1);
        System.out.println("TST has been loaded with " + String.valueOf(legitTrain.size()) + " n-grams"); // This is not
                                                                                                          // the amount
                                                                                                          // of actual
                                                                                                          // n-grams
                                                                                                          // loaded, but
                                                                                                          // I am trying
                                                                                                          // to match
                                                                                                          // TA's output
                                                                                                          // and they
                                                                                                          // did it same
                                                                                                          // as this.
        newTST.evaluateFile(phishingTrain, -1);
        System.out.println("TST has been loaded with " + String.valueOf(phishingTrain.size()) + " n-grams"); // This is
                                                                                                             // not the
                                                                                                             // amount
                                                                                                             // of
                                                                                                             // actual
                                                                                                             // n-grams
                                                                                                             // loaded,
                                                                                                             // but I am
                                                                                                             // trying
                                                                                                             // to match
                                                                                                             // TA's
                                                                                                             // output
                                                                                                             // and they
                                                                                                             // did it
                                                                                                             // same as
                                                                                                             // this.

        this.strongNodes = newTST.getMyStrongs(); // Strong list is set, this will be used when we are discarding
                                                  // insignificant ones. Output is internal.

        newTST.calculateMyWeight(); // Weights of all nodes calculated.
        newTST.printTheWeightsList(); // All nodes are printed with their weights. Output of this method is internal.

        // Discarding insignificant values and getting a final TST.
        int beforeSize = newTST.getAll().size(); // Saving before size for calculation.
        this.learnedTST = newTST.discardInsignificant(newTST, this.strongNodes); // Discarding insignificant nodes if
                                                                                 // they are not a strong one.
        System.out.println(String.valueOf(beforeSize - (learnedTST.getAll().size()))
                + " insignificant n-grams have been removed from the TST"); // May not get exact result, because my
                                                                            // strong list is different, because sorting
                                                                            // matters, in more context;
        // Lets say we have 10 nodes with freq 5,
        // We are selecting last 5 nodes which will be selected from 5 freq,
        // Depending on your sorting algorithm you will select different nodes.
        // From now on output differs.
        // TA did not give any explanation even more avoided answering question via
        // piazza. I gave up matching output.
        //
        // System.out.println(learnedTST.toString());
        // You can print any TST at any point.
        // Alternatively you can print them to a file by writeTSTtoFile(TST theTST,
        // String fileName) method;
        // this.writeTSTtoFile(learnedTST, "result.txt");
        // System.out.println(learnedTST.getAll().size());
        // You can print size of any TST at any point.

        this.testingStart(); // We start testing remaining output is internal. Check down below.
    }

    public void testingStart() {
        for (String string : this.phishingTestFile) { // For every string in the test files.
            int result = this.denoteResult(string); // We denote result.
            if (result == 1) { // and compare if it is indeed true or not, then we increase the test values
                               // accordingly.
                this.True_Positive += 1;
            } else if (result == -1) {
                this.False_Negative += 1;
            } else if (result == 0) {
                this.Unpredictable_Phishing += 1;
            }
        }
        for (String string : this.legitimateTestFile) {
            int result = this.denoteResult(string);
            if (result == -1) {
                this.True_Negative += 1;
            } else if (result == 1) {
                this.False_Positive += 1;
            } else if (result == 0) {
                this.Unpredictable_Legitimate += 1;
            }
        }

        // After we are done we print resulting values.
        System.out.println("TP:" + String.valueOf(this.True_Positive) + " FN:" + String.valueOf(this.False_Negative)
                + " TN:" + String.valueOf(this.True_Negative) + " FP:" + String.valueOf(this.False_Positive)
                + " Unpredictable Phishing:" + String.valueOf(this.Unpredictable_Phishing)
                + " Unpredictable Legitimate:" + String.valueOf(this.Unpredictable_Legitimate));

        // Calculating accuracy and printing.
        float accuracy = (float) (this.True_Positive + this.True_Negative) /
                (float) (this.True_Positive + this.True_Negative + this.False_Positive + this.False_Negative
                        + this.Unpredictable_Phishing + this.Unpredictable_Legitimate);
        System.out.println("Accuracy: " + String.valueOf(accuracy));
    }

    public int denoteResult(String string) {
        List<String> n_grams = this.learnedTST.chopString(string); // Getting all n_grams of the url
        float total = 0;
        for (String gram : n_grams) {
            total += getWeightOfNGram(gram); // For all the n grams getting value and adding it to total.
        }
        if (total > 0) {
            return 1; // minimizing result of total.
        } else if (total < 0) {
            return -1;
        } else if (total == 0) {
            return 0;
        }
        return 0;
    }

    private float getWeightOfNGram(String n_gram) {
        return this.learnedTST.getWeight(n_gram); // Redirecting get weight function to TST one.
    }

    public void writeTSTtoFile(TST theTST, String fileName) { // You can print any TST to a file using this method.
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(outputPath + fileName))));
            // Writing to file
            writer.write(theTST.toString());
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to " + fileName);
            e.printStackTrace();
        }
    }

    public List<String> readFile(String fileName) { // Read file method. Empty lines are not discarded to match output.
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(this.inputPath + fileName),
                Charset.forName(this.inputFileEncoding))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) " + fileName);
        }
        return result;
    }

    /*
     * public String getMidOf(){
     * StringBuilder temp = new StringBuilder();
     * for(int i = 0; i<n_gram ;i++){
     * temp.append("O");
     * }
     * return temp.toString();
     * }
     */

    public static void main(String[] args) {
        Main program = new Main(); // Calling main non-static methods.
        program.start(); // Will do calculations according to Paper, in order to match TA's output.
    }
}