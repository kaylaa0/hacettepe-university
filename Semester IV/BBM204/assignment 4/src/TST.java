/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.util.*;

class Node { // Node class used in TST.
    String data; // n gram value
    int phish_occurrence;
    int legit_occurrence;
    float weight;
    Node left, mid, right;

    public Node(String data, int phish, int legit) { // For initial we input data, 1, 0 for phish learning Node because
                                                     // it's phish occurrence is 1. data, 0, 1 for legit Node.
        this.data = data;
        this.phish_occurrence = phish;
        this.legit_occurrence = legit;
        this.left = null;
        this.mid = null;
        this.right = null;
    }

    public void setWeight(float weight) {
        this.weight = weight;
    }

    public void strip() {
        this.left = null;
        this.mid = null;
        this.right = null;
    }

    public String toPrintString() { // This is not necessary but it is a luxury to print TST when ever we want with
                                    // detailed information.
        return "Node at " + super.toString() + "\n\tNode data: " + this.data + "\n\tphish_occurrence: "
                + String.valueOf(this.phish_occurrence)
                + "\n\tlegit_occurrence: " + String.valueOf(this.legit_occurrence) + "\n\tweight: "
                + String.valueOf(this.weight) +
                "\n\tleft, mid and right nodes: " + this.left + ", " + this.mid + ", " + this.right + "\n";
    }
}

public class TST {

    public Node head; // Head node, in order decrease complexity we add a dummy head.
    private final int FEATURE_SIZE; // current TST's FEATURE_SIZE
    private final int theGram; // current TST's gram. Required in calculations; -1 is max string compare
                               // length.
    public int currentAddingMethod; // -1 for phish +1 for legitimate, 0 for default.
    private final String outputPath;

    public TST(int howManyGrams, int FEATURE_SIZE, String outputPath) {
        this.theGram = howManyGrams;
        this.FEATURE_SIZE = FEATURE_SIZE;
        this.outputPath = outputPath;
    }

    public void evaluateFile(List<String> stringList, int addingMethod) {
        this.currentAddingMethod = addingMethod;
        for (String string : stringList) {
            // string = string.replace("https://","").replace("http://",""); // https:// and
            // http:// in a url is removed according to assignment paper.
            // The line above set to a comment because TA changed it to line below.
            string = string.replace("https", "").replace("http", "").replace("www", ""); // https, http and www removed
                                                                                         // according to a post made by
                                                                                         // TA at piazza.
            string = string.toLowerCase(); // url is lowercased.
            List<String> resultedNGrams = chopString(string); // n grams a driven from given url.
            for (String ngram : resultedNGrams) {
                Node aNewNode; // For each n gram we create a new node and add it to TST. Node is created with
                               // initial phish or legit value according to which file we are currently
                               // learning.
                if (this.currentAddingMethod == -1) {
                    aNewNode = new Node(ngram, 1, 0);
                } else if (this.currentAddingMethod == 1) {
                    aNewNode = new Node(ngram, 0, 1);
                } else {
                    continue;
                }
                this.addNode(aNewNode);
            }
        }
    }

    public List<String> chopString(String aString) { // String to n grams list converting method.
        List<String> result = new ArrayList<String>();
        for (int startletterno = 0; startletterno <= aString.length() - this.theGram; startletterno++) { // For every
                                                                                                         // letter until
                                                                                                         // we can not
                                                                                                         // fit a n
                                                                                                         // gram.
            StringBuilder tempGram = new StringBuilder();
            for (int offset = 0; offset < this.theGram; offset++) { // We add letters in range of n gram to a string.
                tempGram.append(aString.charAt(startletterno + offset));
            }
            result.add(tempGram.toString());
        }
        return result;
    }

    public List<Node> getAll() { // Returns left, mid, right traversal array of the TST.
        List<Node> returnList = new ArrayList<Node>();
        traverseNode(this.head, returnList);
        return returnList;
    }

    private void traverseNode(Node node, List<Node> theList) {
        if (node.left != null) { // If child nodes are not null first we check them.
            traverseNode(node.left, theList);
        }
        if (node.mid != null) {
            traverseNode(node.mid, theList);
        }
        if (node.right != null) {
            traverseNode(node.right, theList);
        }
        theList.add(node); // Add node to list.
    }

    public List<Node> getMyStrongs() { // Will return top FEATURE_SIZE frequent phish and legit n grams, concerted in a
                                       // list.
        List<Node> phishing = this.getAll(); // First we divide TST.
        List<Node> legitimate = this.getAll();
        this.discardInsignificant(phishing, legitimate); // Discarding non frequent (insignificant) ones.
        this.printMyStrongs(phishing, legitimate); // Printing result files.
        for (Node node : legitimate) {
            if (phishing.contains(node)) {
                continue;
            } else {
                phishing.add(node); // Concentrating lists.
            }
        }
        return phishing;
    }

    private void discardInsignificant(List<Node> phishing, List<Node> legitimate) {
        phishing.sort((o1, o2) -> ((Integer) o2.phish_occurrence).compareTo(o1.phish_occurrence)); // We reverse sort
                                                                                                   // list. This method
                                                                                                   // is very simple.
                                                                                                   // However sadly it
                                                                                                   // is not TA's method
                                                                                                   // so output will
                                                                                                   // differ.
        legitimate.sort((o1, o2) -> ((Integer) o2.legit_occurrence).compareTo(o1.legit_occurrence));
        for (int i = phishing.size() - 1; i >= this.FEATURE_SIZE; i--) { // For item from end of the list to
                                                                         // FEATURE_SIZE we remove.
            phishing.remove(i);
        }
        for (int i = legitimate.size() - 1; i >= this.FEATURE_SIZE; i--) {
            legitimate.remove(i);
        }
    }

    private void printMyStrongs(List<Node> phishing, List<Node> legitimate) { // Prints both Strong lists.
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(
                    new FileOutputStream(new File(outputPath + "strong_phishing_features.txt"))));
            // Writing to file
            writer.write("Most important phishing n_grams\n");
            for (int i = 0; i < phishing.size(); i++) {
                writer.write(String.valueOf(i + 1) + ". ");
                writer.write(phishing.get(i).data);
                writer.write(" - freq: ");
                writer.write(String.valueOf(phishing.get(i).phish_occurrence));
                writer.write("\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to strong_phishing_features.txt at " + outputPath);
            e.printStackTrace();
        } catch (IndexOutOfBoundsException e) {
            System.err.println(
                    "Problem writing to strong_phishing_features.txt. Your chosen FEATURE_SIZE is bigger than all n grams count.");
            e.printStackTrace();
        }
        System.out.println(String.valueOf(phishing.size())
                + " strong phishing n-grams have been saved to the file\"strong_phishing_features.txt\""); // Prints
                                                                                                           // output to
                                                                                                           // console.
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(
                    new FileOutputStream(new File(outputPath + "strong_legitimate_features.txt"))));
            // Writing to file
            writer.write("Most important legitimate n_grams\n");
            for (int i = 0; i < legitimate.size(); i++) {
                writer.write(String.valueOf(i + 1) + ". ");
                writer.write(legitimate.get(i).data);
                writer.write(" - freq: ");
                writer.write(String.valueOf(legitimate.get(i).legit_occurrence));
                writer.write("\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to strong_legitimate_features.txt at " + outputPath);
            e.printStackTrace();
        } catch (IndexOutOfBoundsException e) {
            System.err.println(
                    "Problem writing to strong_legitimate_features.txt. Your chosen FEATURE_SIZE is bigger than all n grams count.");
            e.printStackTrace();
        }
        System.out.println(String.valueOf(legitimate.size())
                + " strong legitimate n-grams have been saved to the file \"strong_phishing_features.txt\""); // There
                                                                                                              // is an
                                                                                                              // extra
                                                                                                              // space
                                                                                                              // here,
                                                                                                              // but TA
                                                                                                              // has one
                                                                                                              // too!!
    }

    public void calculateMyWeight() { // Calculates all nodes weights.
        this.weightTraverse(this.head); // Calls a traversing method.
    }

    private void weightTraverse(Node node) {
        if (node.left != null) {
            weightTraverse(node.left);
        }
        if (node.mid != null) {
            weightTraverse(node.mid);
        }
        if (node.right != null) {
            weightTraverse(node.right);
        }
        this.calculateWeight(node); // Traversing the TST we call calculate weight method for all nodes.
    }

    private void calculateWeight(Node node) { // This function is given in assignment paper.
        if (node.phish_occurrence > 0 && node.legit_occurrence == 0)
            node.setWeight(1); // a very unique n-gram for phishing medium
        else if (node.phish_occurrence == 0 && node.legit_occurrence > 0)
            node.setWeight(-1); // a very unique n-gram for legitimate medium
        else if (node.phish_occurrence > 0 && node.legit_occurrence > 0) {
            if (node.phish_occurrence > node.legit_occurrence) {
                node.setWeight((float) Math.min(node.phish_occurrence, node.legit_occurrence)
                        / (float) Math.max(node.phish_occurrence, node.legit_occurrence)); // (0,1)
            } else if (node.phish_occurrence < node.legit_occurrence) {
                node.setWeight(-((float) ((float) Math.min(node.phish_occurrence, node.legit_occurrence)
                        / (float) Math.max(node.phish_occurrence, node.legit_occurrence)))); // (-1,0)
            } else {
                node.setWeight(0); // the n-gram appears equally in both of the mediums
            }
        }
    }

    public void printTheWeightsList() {
        List<Node> allNodes = this.getAll(); // First we get list of all nodes.
        allNodes.sort((o1, o2) -> ((Float) o2.weight).compareTo((Float) o1.weight)); // Sort them in descending order.
                                                                                     // Again for the same values it is
                                                                                     // impossible match TA's result
                                                                                     // because he didn't state a rule
                                                                                     // nor his solution so as long as
                                                                                     // we don't get lucky or have
                                                                                     // ability to read TA's mind we
                                                                                     // won't be able match result.
        try {
            Writer writer = new BufferedWriter(
                    new OutputStreamWriter(new FileOutputStream(new File(outputPath + "all_feature_weights.txt"))));
            // Writing to file
            writer.write("All N-Gram Weights\n");
            for (int i = 0; i < allNodes.size(); i++) {
                writer.write(allNodes.get(i).data);
                writer.write(" - weight: ");
                writer.write(String.valueOf(allNodes.get(i).weight));
                writer.write("\n");
            }
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing to all_feature_weights.txt at " + outputPath);
            e.printStackTrace();
        }
        System.out.println(String.valueOf(allNodes.size())
                + " n-grams + weights have been saved to the file \"all_feature_weights.txt\""); // Printing output.
    }

    public TST discardInsignificant(TST theTST, List<Node> nodeList) { // Discarding Insignificant nodes and returning
                                                                       // resulting new TST.
        TST tempTST = new TST(theTST.theGram, theTST.FEATURE_SIZE, theTST.outputPath); // First we create a empty clone
                                                                                       // of our initial TST.
        this.traverseForDiscard(theTST.head, tempTST, nodeList); // We use strong nodes list to determine if they are
                                                                 // significant or not.
        return tempTST;
    }

    public void traverseForDiscard(Node node, TST toAddTST, List<Node> nodeList) { // Traversing all of the list if a
                                                                                   // node is also significant ( in
                                                                                   // either of top frequency lists ) we
                                                                                   // add it to our new TST.
        // Here the shape of the new list will be warped. Again without knowing TA's
        // implementation it is impossible to match.
        // We first traverse mid in order to have some degree balance.
        if (node.mid != null) {
            traverseForDiscard(node.mid, toAddTST, nodeList);
        }
        if (node.left != null) {
            traverseForDiscard(node.left, toAddTST, nodeList);
        }
        if (node.right != null) {
            traverseForDiscard(node.right, toAddTST, nodeList);
        }
        if (nodeList.contains(node)) {
            node.strip(); // Before adding node to new TST first we strip it's children resulting in a
                          // rebirth.
            toAddTST.addNode(node);
        }
    }

    public void addNode(Node toAdd) { // Adds a node to TST.
        if (this.head == null) { // If head is null we add it to head.
            this.head = toAdd;
        } else {
            checkNode(toAdd, this.head, 0); // Other wise we will search for a correct empty spot.
        }
    }

    private void checkNode(Node toInsert, Node toCompare, int nthLetter) {
        // Developer notes: "if a coto b if b>a -1, b<a 1", toCompare is current node,
        // toInsert is the one we are trying to add.
        // Notes to TA:
        // Method can be summed like this, first we check nth letter and move
        // accordingly in the list. N starts from 0, if we have a match we move to mid
        // of TST and increase n by 1 so we know we are dealing with next letter.
        // We also check whenever there is a match, if it is a total match. If it is
        // then instead we increase the according values of node.
        if (Character.toString(toCompare.data.charAt(nthLetter))
                .compareTo(Character.toString(toInsert.data.charAt(nthLetter))) < 0) { // Will be placed to right.
            if (toCompare.right == null) { // If node is empty we occupy it.
                toCompare.right = toInsert;
            } else {
                checkNode(toInsert, toCompare.right, nthLetter); // If not we continue search.
            }
        } else if (Character.toString(toCompare.data.charAt(nthLetter))
                .compareTo(Character.toString(toInsert.data.charAt(nthLetter))) == 0) {
            if (toCompare.data.compareTo(toInsert.data) == 0) { // Total match is found, instead of placing node will
                                                                // increase the value.
                if (currentAddingMethod == -1) { // Currently reading a phishing file.
                    toCompare.phish_occurrence += 1; // Increasing phish total.
                } else if (currentAddingMethod == 1) { // Currently reading a legitimate file.
                    toCompare.legit_occurrence += 1; // Increasing legit total.
                }
            } else { // Moving to mid of the node, in tree.
                if (toCompare.mid == null) { // If node is null we occupy it.
                    toCompare.mid = toInsert;
                } else { // If not we continue our search.
                    checkNode(toInsert, toCompare.mid, nthLetter + 1); // nthLetter is increased,
                    // because we know the current letter is a match and all the letters of nth
                    // position in mid branch are going to be a match,
                    // now we need to check for next letter.
                }
            }
        } else if ((Character.toString(toCompare.data.charAt(nthLetter))
                .compareTo(Character.toString(toInsert.data.charAt(nthLetter))) > 0)) { // Will be moving left.
            if (toCompare.left == null) {
                toCompare.left = toInsert;
            } else {
                checkNode(toInsert, toCompare.left, nthLetter);
            }
        }
    }

    public float getWeight(String n_gram) { // Gets weight of a n gram
        Node foundNode = this.traverseGetWeight(this.head, n_gram, 0); // We traverse the TST to find n-gram. This is a
                                                                       // advanced search method rather than traversing
                                                                       // all the list.
        // It would mean nothing if we search all nodes. We create TST in order to have
        // this advanced search method.
        if (foundNode == null) { // If node is null that means we couldn't find a match, in other words the
                                 // current n grams place in TST was empty.
            return 0;
        } else {
            return foundNode.weight; // If found match we return node's weight value.
        }
    }

    private Node traverseGetWeight(Node node, String toCompare, int nthLetter) {
        // This method is similar to how we construct our TST.
        // We check letter by letter and move right or left or mid. If we move mid,
        // meaning we had a letter match we check for next letter. Also we check if it
        // is a total match.
        // If it is a total match we return node. Otherwise we move to mid node.
        // If the next mid node is null it will be caught at the beginning of method and
        // return null.
        if (node == null) {
            return null;
        }
        if (Character.toString(node.data.charAt(nthLetter))
                .compareTo(Character.toString(toCompare.charAt(nthLetter))) == 0) {
            if (node.data.compareTo(toCompare) == 0) {
                return node;
            } else {
                return traverseGetWeight(node.mid, toCompare, nthLetter + 1);
            }
        } else if (Character.toString(node.data.charAt(nthLetter))
                .compareTo(Character.toString(toCompare.charAt(nthLetter))) > 0) {
            return traverseGetWeight(node.left, toCompare, nthLetter);
        } else if (Character.toString(node.data.charAt(nthLetter))
                .compareTo(Character.toString(toCompare.charAt(nthLetter))) < 0) {
            return traverseGetWeight(node.right, toCompare, nthLetter);
        }
        return null;
    }

    @Override
    public String toString() { // It is a luxury to be able to call toString on any object and get detailed
                               // correct result.
        StringBuilder builder = new StringBuilder();
        traverseOverridePrint(this.head, builder);
        return builder.toString();
    }

    private void traverseOverridePrint(Node node, StringBuilder builder) {
        if (node.left != null) {
            traverseOverridePrint(node.left, builder);
        }
        if (node.mid != null) {
            traverseOverridePrint(node.mid, builder);
        }
        if (node.right != null) {
            traverseOverridePrint(node.right, builder);
        }
        builder.append(node.toPrintString());
    }
}
