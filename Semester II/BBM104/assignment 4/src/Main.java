/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.ThreadLocalRandom;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) {
        Main program = new Main();
        QueueOperator queueOperator = new QueueOperator(); // We create operators.
        StackOperator stackOperator = new StackOperator();
        List<String> input = program.readFile(args[0]); // Read input file.
        Queue<Integer> theQueue = queueOperator.initQueFromFile(program.readFile("queue.txt")); // read queue and stack
                                                                                                // file and initialize
                                                                                                // them.
        Stack<Integer> theStack = stackOperator.initStackFromFile(program.readFile("stack.txt"));

        try {
            Writer qWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("queueOut.txt")))); // Setting
                                                                                                                         // up
                                                                                                                         // writers.
            Writer sWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File("stackOut.txt"))));

            for (String line : input) {
                String[] parts = line.split(" "); // Dividing input line to parts.
                if (parts[0].toLowerCase().equals("q")) { // Checking first part and continuing accordingly.
                    if (parts[1].toLowerCase().equals("removegreater")) { // Checking second part and calling operation
                                                                          // from operator and also printing output to
                                                                          // file.
                        queueOperator.removeGreater(theQueue, Integer.parseInt(parts[2]));
                        qWriter.write("After " + parts[1] + " " + parts[2] + ":");
                        qWriter.write("\n" + theQueue.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("calculatedistance")) {
                        int result = queueOperator.calculateDistance(theQueue);
                        qWriter.write("After " + parts[1] + ":");
                        qWriter.write("\nTotal distance=" + String.valueOf(result) + "\n");
                    } else if (parts[1].toLowerCase().equals("addorremove")) {
                        queueOperator.addOrRemove(theQueue, Integer.parseInt(parts[2]));
                        qWriter.write("After " + parts[1] + " " + parts[2] + ":");
                        qWriter.write("\n" + theQueue.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("reverse")) {
                        queueOperator.reverseElements(theQueue, Integer.parseInt(parts[2]));
                        qWriter.write("After " + parts[1] + " " + parts[2] + ":");
                        qWriter.write("\n" + theQueue.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("sortelements")) {
                        queueOperator.sortElements(theQueue);
                        qWriter.write("After " + parts[1] + ":");
                        qWriter.write("\n" + theQueue.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("distinctelements")) {
                        int result = queueOperator.distinctElements(theQueue);
                        qWriter.write("After " + parts[1] + ":");
                        qWriter.write("\nTotal distinct element=" + String.valueOf(result) + "\n");
                    }
                } else if (parts[0].toLowerCase().equals("s")) {
                    if (parts[1].toLowerCase().equals("removegreater")) {
                        stackOperator.removeGreater(theStack, Integer.parseInt(parts[2]));
                        sWriter.write("After " + parts[1] + " " + parts[2] + ":");
                        sWriter.write("\n" + theStack.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("calculatedistance")) {
                        int result = stackOperator.calculateDistance(theStack);
                        sWriter.write("After " + parts[1] + ":");
                        sWriter.write("\nTotal distance=" + String.valueOf(result) + "\n");
                    } else if (parts[1].toLowerCase().equals("addorremove")) {
                        stackOperator.addOrRemove(theStack, Integer.parseInt(parts[2]));
                        sWriter.write("After " + parts[1] + " " + parts[2] + ":");
                        sWriter.write("\n" + theStack.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("reverse")) {
                        stackOperator.reverseElements(theStack, Integer.parseInt(parts[2]));
                        sWriter.write("After " + parts[1] + " " + parts[2] + ":");
                        sWriter.write("\n" + theStack.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("sortelements")) {
                        stackOperator.sortElements(theStack);
                        sWriter.write("After " + parts[1] + ":");
                        sWriter.write("\n" + theStack.toString() + "\n");
                    } else if (parts[1].toLowerCase().equals("distinctelements")) {
                        int result = stackOperator.distinctElements(theStack);
                        sWriter.write("After " + parts[1] + ":");
                        sWriter.write("\nTotal distinct element=" + String.valueOf(result) + "\n");
                    }
                } else {
                    System.err.println("First char of input line should be Q or S.");
                }
            }

            qWriter.close(); // Closing output files.
            sWriter.close();
        } catch (IOException e) {
            System.err.println("Problem writing to output files.");
            e.printStackTrace();
        }

        theQueue.save("queue.txt"); // Saving final stack and queue files.
        theStack.save("stack.txt");
    }

    public List<String> readFile(String fileName) { // Read file method.
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get(fileName))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) " + fileName);
        }
        return result;
    }
}

class QueueOperator {
    public Queue<Integer> initQueFromFile(List<String> file) { // Very basic, we get integers from file and add
                                                               // accordingly.
        Queue<Integer> newQueue = new Queue<Integer>();
        for (String line : file) {
            String[] seperated = line.split(" ");
            for (String number : seperated) {
                newQueue.enQueue(Integer.valueOf(number));
            }
        }
        return newQueue;
    }

    public void reverseQueue(Queue<Integer> queue) { // Reversing a queue or a stack means adding it to a stack or a
                                                     // queue and adding it back.
        Stack<Integer> tempStack = new Stack<Integer>();
        while (!queue.isEmpty()) { // This is the method I use everywhere in this assignment to empty out any type
                                   // of data structure.
            tempStack.push(queue.deQueue());
        }
        while (!tempStack.isEmpty()) {
            queue.enQueue(tempStack.pop());
        }
    }

    public void removeGreater(Queue<Integer> queue, int n) {
        Queue<Integer> newQueue = new Queue<Integer>();
        int size = queue.getSize();
        for (int i = 0; i < size; i++) {
            int removingNumber = queue.remove(); // All elements are removed and added to a temp queue if they are
                                                 // greater.
            if (removingNumber <= n) {
                newQueue.add(removingNumber);
            }
        }
        while (!newQueue.isEmpty()) { // Finally we add them back.
            queue.enQueue(newQueue.deQueue());
        }
    }

    public int calculateDistance(Queue<Integer> queue) {
        Stack<Integer> tempStack = new Stack<Integer>();
        int size = queue.getSize();
        int calculation = 0;
        for (int i = 1; i < size; i++) {
            for (int a = 0; a < i; a++) {
                tempStack.push(queue.deQueue());
            }
            int currentNumber = tempStack.peek(); // This one is a more complicated algorithm. In order to sum it up;
                                                  // Every time we add 1 more item to tempStack until end of the list,
                                                  // after we add an item we peek at it to see. At first we add 1 item
                                                  // and see it, for example. And then we add rest of the items. And
                                                  // while removing those items back to the list we first peek at them
                                                  // and calculate distance with current item.
            while (!queue.isEmpty()) {
                tempStack.push(queue.deQueue());
            }
            for (int b = 0; b < size - i; b++) {
                calculation += Math.abs(currentNumber - tempStack.peek());
                queue.enQueue(tempStack.pop()); // After adding the distance
            }
            while (!tempStack.isEmpty()) {
                queue.enQueue(tempStack.pop()); // We put the rest of the items back
            }
            this.reverseQueue(queue); // In this process queue gets reversed so we reverse it back. Next iteration we
                                      // add 1 more item and we peek 1 less item at the end.
        }
        return calculation;
    }

    public void addOrRemove(Queue<Integer> queue, int k) {
        if (k > 0) { // First we check either we add or remove.
            while (k > 0) { // Until sufficient amount we do the operation.
                queue.enQueue(ThreadLocalRandom.current().nextInt(0, 51)); // This is random 0-50
                k--;
            }
        } else if (k < 0) {
            while (k < 0) {
                queue.deQueue();
                k++;
            }
        }
    }

    public void reverseElements(Queue<Integer> queue, int k) {
        Stack<Integer> tempStack = new Stack<Integer>();
        if (k > queue.getSize()) {
            System.err.println("k is bigger than size, k is set to size."); // This is an error check just in case.
            k = queue.getSize(); // Lets say we have n elements, if k is bigger than n it is illogical so we
                                 // reverse list.
        }
        for (int i = 0; i < k; i++) {
            tempStack.push(queue.deQueue()); // For queue we put them in stack first.
        }
        Queue<Integer> tempQueue = new Queue<>();
        while (!tempStack.isEmpty()) {
            tempQueue.enQueue(tempStack.pop()); // Than we put that items from stack to another queue.
        }
        while (!queue.isEmpty()) {
            tempQueue.enQueue(queue.deQueue()); // Rest of the queue is putted to other queue.
        }
        while (!tempQueue.isEmpty()) {
            queue.enQueue(tempQueue.deQueue()); // Other queue is final so it is copied to main queue.
        }
    }

    public void sortElements(Queue<Integer> queue) {
        List<Integer> listToSort = new ArrayList<>(); // TA said this is legit method so, first we get all items to
                                                      // array list.
        while (!queue.isEmpty()) {
            listToSort.add(queue.deQueue());
        }
        Collections.sort(listToSort); // Sort array list.
        while (!listToSort.isEmpty()) {
            queue.enQueue(listToSort.remove(0)); // Put it back.
        }
    }

    public int distinctElements(Queue<Integer> queue) {
        Set<Integer> tempSet = new HashSet<Integer>(); // TA said this is also legit method so, first we get all items
                                                       // to a set.
        Queue<Integer> tempQueue = new Queue<>();
        while (!queue.isEmpty()) {
            tempSet.add(queue.peek());
            tempQueue.enQueue(queue.deQueue()); // While doing it we peek for set, and move item to a secure place, we
                                                // don't want to mess up our main queue.
        }
        while (!tempQueue.isEmpty())
            queue.enQueue(tempQueue.deQueue()); // Moving items from secure place back to main.
        return tempSet.size(); // Returning sets size.
    }
}

class StackOperator { // For stack most of the stuff are same.
    public Stack<Integer> initStackFromFile(List<String> file) {
        Stack<Integer> newStack = new Stack<Integer>();
        for (String line : file) {
            String[] seperated = line.split(" ");
            for (String number : seperated) {
                newStack.push(Integer.valueOf(number));
            }
        }
        this.reverseStack(newStack);
        return newStack;
    }

    public void reverseStack(Stack<Integer> stack) {
        Queue<Integer> tempQueue = new Queue<Integer>();
        while (!stack.isEmpty()) {
            tempQueue.enQueue(stack.pop()); // Opposite of what we do in queue.
        }
        while (!tempQueue.isEmpty()) {
            stack.push(tempQueue.deQueue());
        }
    }

    public void removeGreater(Stack<Integer> stack, int n) {
        Stack<Integer> newStack = new Stack<Integer>();
        int size = stack.getSize();
        for (int i = 0; i < size; i++) {
            int removingNumber = stack.pop();
            if (removingNumber <= n) {
                newStack.push(removingNumber);
            }
        }
        while (!newStack.isEmpty()) {
            stack.push(newStack.pop());
        }
    }

    public int calculateDistance(Stack<Integer> stack) {
        Stack<Integer> tempStack = new Stack<Integer>();
        int size = stack.getSize();
        int calculation = 0;
        for (int i = 1; i < size; i++) {
            for (int a = 0; a < i; a++) {
                tempStack.push(stack.pop());
            }
            int currentNumber = tempStack.peek();
            while (!stack.isEmpty()) {
                tempStack.push(stack.pop());
            }
            for (int b = 0; b < size - i; b++) {
                calculation += Math.abs(currentNumber - tempStack.peek());
                stack.push(tempStack.pop());
            }
            while (!tempStack.isEmpty()) {
                stack.push(tempStack.pop());
            }
            // No need to reverse here because it comes ordered.
        }
        return calculation;
    }

    public void addOrRemove(Stack<Integer> stack, int k) {
        if (k > 0) {
            while (k > 0) {
                stack.push(ThreadLocalRandom.current().nextInt(0, 51));
                k--;
            }
        } else if (k < 0) {
            while (k < 0) {
                stack.pop();
                k++;
            }
        }
    }

    public void reverseElements(Stack<Integer> stack, int k) {
        Queue<Integer> tempQueue = new Queue<>(); // Opposite of what we do in queue.
        if (k > stack.getSize()) {
            System.err.println("k is bigger than size, k is set to size.");
            k = stack.getSize();
        }
        for (int i = 0; i < k; i++) {
            tempQueue.enQueue(stack.pop()); // We don't copy them to another stack instead we put right back.
        } // This is different because when we put back to queue it puts to the end but
          // when we put back to stack it puts right back.
        while (!tempQueue.isEmpty()) {
            stack.push(tempQueue.deQueue());
        }
    }

    public void sortElements(Stack<Integer> stack) {
        List<Integer> listToSort = new ArrayList<>();
        while (!stack.isEmpty()) {
            listToSort.add(stack.pop());
        }
        Collections.sort(listToSort);
        while (!listToSort.isEmpty()) {
            stack.push(listToSort.remove(0));
        }
        this.reverseStack(stack);
    }

    public int distinctElements(Stack<Integer> stack) {
        Set<Integer> tempSet = new HashSet<Integer>();
        Stack<Integer> tempStack = new Stack<>();
        while (!stack.isEmpty()) {
            tempSet.add(stack.peek());
            tempStack.push(stack.pop());
        }
        while (!tempStack.isEmpty())
            stack.push(tempStack.pop());
        return tempSet.size();
    }
}