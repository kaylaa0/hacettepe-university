public class Main {
    public class Node {
        public int data;
        public Node next;
        public Node prev;

        public Node(int data) {
            this.data = data;
            this.next = this.prev = null;
        }
    }

    public class List {
        Node head;

        public List() {
            head = null;
        }

        public void insertAtEnd(Node newNode) {
            Node current = this.head;
            while (current.next != null) {
                current = current.next;
            }
            newNode.prev = current;
            newNode.next = null;
            current.next = newNode;

        }

        public void insertBefore(Node preNode, Node newNode) {
            Node current = this.head;
            while (current.next != null) {
                if (current.next == preNode) { // Is at the middle anyways so is not checking if it is the head. Or
                                               // adding to end is not used because in exam paper it is stated
                                               // "intermediate".
                    preNode.prev = newNode;
                    newNode.prev = current;
                    newNode.next = preNode;
                    current.next = newNode;
                    break;
                }
                current = current.next;
            }

        }
    }

    public void start() {
        System.out.println(1);
    }

    public static void main(String[] args) {
        Main program = new Main();
        program.start();
    }
}
