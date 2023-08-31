/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.lang.Comparable;

public class BST<Key extends Comparable<Key>, Value> {

    public Node root;

    public class Node {
        public Key key;
        public Value val;
        public Node left, right;

        public Node(Key key, Value val) {
            this.key = key;
            this.val = val;
        }
    }

    public Value get(Key key) {
        while (root != null) {
            if (key.compareTo(root.key) > 0)
                root = root.right;
            else if (key.compareTo(root.key) < 0)
                root = root.left;
            else
                return root.val;
        }
        return root.val;
    }

    public void put(Key key, Value val) {
        Node curr = root;
        boolean override = false;
        Node parent = null;
        if (root == null) {
            root = new Node(key, val);
            return;
        }

        while (curr != null) {
            parent = curr;
            if (key.compareTo(curr.key) < 0) {
                curr = curr.left;
            } else if (key.compareTo(curr.key) > 0) {
                curr = curr.right;
            } else if (key.compareTo(curr.key) == 0) {
                override = true;
                break;
            }
        }

        if (override) {
            curr.val = val;
        } else {
            if (key.compareTo(parent.key) < 0) {
                parent.left = new Node(key, val);
            } else {
                parent.right = new Node(key, val);
            }
        }
    }

    public static void main(String[] args) {
        BST program = new BST();
        program.start();
    }

    public void start() {
        BST bst = new BST();
        BST.Node[] nodes = { new BST.Node(2, 0), new BST.Node(7, 1), new BST.Node(4, 2), new BST.Node(88, 3),
                new BST.Node(4, 4), new BST.Node(2, 5), new BST.Node(7, 6) };
        for (BST.Node node : nodes) {
            bst.put(node.key, node.val);
        }
        Object temp = bst.get(7);
        temp = (Integer) temp;
        System.out.println(temp);
    }
}
