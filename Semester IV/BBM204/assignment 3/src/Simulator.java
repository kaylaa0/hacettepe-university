/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

public class Simulator {
    public Graph graph; // Graph used for first part of question
    private Queue<String> threadQueue = new LinkedList<String>(); // Queues for BFS
    public Node startNode; // Start node for wireless search
    public Queue<String> messagePassing = new LinkedList<String>(); // Message passing queue, we sort it reversed and
                                                                    // then pass messages to parent.
    public Graph tree; // Tree used fore second part of assignment

    public void setGraph(Graph toSet) {
        this.graph = toSet;
    }

    public void setTree(Graph tree) {
        this.tree = tree;
    }

    public void setStartNode(Node startNode) {
        this.startNode = startNode;
    }

    public String startSimulation() { // Main simulation for first part of question, will simulate wireless systems
                                      // data exchange.
        StringBuilder builder = new StringBuilder("Broadcast steps:\n");
        threadQueue.add(startNode.getName());
        startNode.setActiveActiv();
        while (!threadQueue.isEmpty()) { // Namely we perform advanced BFS
            String current = threadQueue.remove();
            simulate(graph.getNode(current), builder);
        }
        return builder.toString();
    }

    private boolean isDone() { // This function is no longer used in the assignment, just in case I keep it.
        for (int i = 0; i < this.graph.getSize(); i++) {
            if (!this.graph.getNode(i).isActive) {
                return false;
            }
        }
        return true;
    }

    public void simulate(Node node, StringBuilder builder) { // Inside of advanced BFS for wireless data exchange.
        builder.append(node.getName()).append("-->");
        boolean isAny = false;
        for (String a : node.adjecencyList) { // For every adjacent vertex
            if (graph.getNode(a).isActive) { // If it is marked we return
                continue;
            } else {
                isAny = true; // This is set true so we know we did perform operation for this item in queue,
                              // the output will be printed accordingly
                this.tree.addEdge(new Edge(node.getName(), a)); // Adding an edge to the tree for second half of
                                                                // assignment
                builder.append(a).append(" ");
                messagePassing.add(a); // Message passing queue gets current element
                graph.getNode(a).setActiveActiv(); // Set marked
                graph.getNode(a).setParent(node); // Parent is stored
                this.threadQueue.add(a); // Adding them to the queue
            }
        }
        if (!isAny) {
            builder.delete(builder.length() - 1, builder.length());
            builder.delete(builder.length() - 1, builder.length());// If current queue item is not operated deleting
                                                                   // printed false parts of string.
            builder.delete(builder.length() - 1, builder.length());
            builder.delete(builder.length() - 1, builder.length());
        } else {
            builder.delete(builder.length() - 1, builder.length()); // Trying to match output.txt
            builder.append("\n");
        }
    }

    public String passMessageSimulation() { // Passing message to parent nodes
        reverseQueue(messagePassing); // First reversing message passing queue because message will passed from last
                                      // added element to first added
        StringBuilder builder = new StringBuilder("Message passing:\n");
        while (!messagePassing.isEmpty()) {
            builder.append(this.passMessage(messagePassing.remove())); // Passing message and removing from queue and
                                                                       // building output in one line.
        }
        builder.append("Best node-->");
        for (Bottle bottle : this.startNode.message.bestList) {
            builder.append(bottle.nodeName).append(", "); // Output stuff
        }
        builder.delete(builder.length() - 1, builder.length()); // Matching output
        builder.delete(builder.length() - 1, builder.length());
        builder.append("\n");
        return builder.toString();
    }

    public String passMessage(String nodeName) {
        StringBuilder returnString = new StringBuilder(nodeName);
        returnString.append("--->");
        this.graph.getNode(nodeName).craftMessage(); // First current nodes message created, meaning current nodes data
                                                     // added to message.
        returnString.append(this.graph.getNode(nodeName).getMessage().toString());
        this.graph.getNode(nodeName).parent.receiveMessage(this.graph.getNode(nodeName).getMessage()); // Then the
                                                                                                       // message is
                                                                                                       // received by
                                                                                                       // parent node.
        returnString.append("--->").append(this.graph.getNode(nodeName).parent.getName()).append("\n");
        return returnString.toString();
    }

    public void reverseQueue(Queue<String> queue) // Reversing a queue, first put in stack and put in queue again.
    {
        Stack<String> stack = new Stack<>();
        while (!queue.isEmpty()) {
            stack.add(queue.peek());
            queue.remove();
        }
        while (!stack.isEmpty()) {
            queue.add(stack.peek());
            stack.pop();
        }
    }

    public String rootCalculation() { // Second half of question.
        StringBuilder builder = new StringBuilder("Possible roots-->");
        Node firstIter = this.BFS(startNode); // Doing double BFS to getting longest path
        this.clearTree(); // In between BFSs clearing tree.
        Node longestPathEdge = this.BFS(firstIter); // Second BFS, firstIter and longestPathEdge are the longest path's
                                                    // vertices.
        int longestPathLength = longestPathEdge.depth + 1; // Calculating longest path's length.
        if (longestPathLength % 2 == 1) { // If longest path is not multiple of 2 we only have 1 possible root.
            builder.append(this.getNthParent(longestPathEdge, (longestPathLength - 1) / 2).getName());
        } else if (longestPathLength % 2 == 0) { // If longest path is multiple of 2 we have 2 possible roots.
            builder.append(this.getNthParent(longestPathEdge, longestPathLength / 2).getName());
            builder.append(", ");
            builder.append(this.getNthParent(longestPathEdge, (longestPathLength / 2) - 1).getName());
        }
        return builder.toString();
    }

    public Node getNthParent(Node node, int n) { // There is no need to create a path, path is stored in this way: from
                                                 // last vertex we trace parent for BFS, when we reach a null parent
                                                 // that is complete path.
        Node current = node;
        for (int i = 0; i < n; i++) { // Going parent node desired time, used for going to middle of the path and
                                      // getting roots.
            current = current.parent;
        }
        return current;
    }

    public void clearTree() {
        for (int i = 0; i < this.tree.getSize(); i++) { // Clearing tree for another BFS operation
            this.tree.getNode(i).setActiveDeActiv();
            this.tree.getNode(i).setParent(null);
            this.tree.getNode(i).setDepth(0);
        }
    }

    public Node BFS(Node beginningNode) { // Basic BFS for second part with tree
        beginningNode.setActiveActiv();
        beginningNode.setDepth(0);
        this.threadQueue.add(beginningNode.getName());
        while (!this.threadQueue.isEmpty()) {
            Node node = this.tree.getNode(this.threadQueue.remove());
            for (String a : node.adjecencyList) {
                if (tree.getNode(a).isActive) {
                    continue;
                } else {
                    this.threadQueue.add(a);
                    tree.getNode(a).setActiveActiv();
                    tree.getNode(a).setParent(node);
                    tree.getNode(a).setDepth(node.depth + 1);
                }
            }
        }
        Node current = beginningNode;
        for (int i = 0; i < this.tree.getSize(); i++) { // We get deepest node, doing this 2 times ensures to gain
                                                        // longest path.
            if (this.tree.getNode(i).depth > current.depth) {
                current = this.tree.getNode(i);
            }
        }
        return current;
    }
}
