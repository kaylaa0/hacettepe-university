/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class LinkedList<T> {
    // A basic linked list class. However some methods are not used I still keep
    // them.
    protected Node<T> head, tail;
    protected long size;

    public LinkedList() {
        head = null;
        tail = null;
        size = 0;
    }

    public void addHead(Node<T> node) {
        if (tail == null)
            tail = node;
        node.setNext(head);
        head = node;
        size++;
    }

    public void addNext(Node<T> currentNode, Node<T> nextNode) {
        if (currentNode == tail)
            tail = nextNode;
        nextNode.setNext(currentNode.getNext());
        currentNode.setNext(nextNode);
        size++;
    }

    public void addTail(Node<T> Node) {
        Node.setNext(null);
        tail.setNext(Node);
        tail = Node;
        size++;
    }

    public boolean remove(int index) {
        Node<T> before, current;

        if (size == 0) {
            System.err.println("Can not remove from an empty list.");
            return false;
        } else {
            if (index == 0) {
                removeFirst();
                return true;
            } else if (index == this.size - 1) {
                removeLast();
                return true;
            } else {
                before = getFirst();
                current = getFirst().getNext();
                for (int count = 0; count < index; count++) {
                    if (count == index - 1) {
                        before.setNext(current.getNext());
                        size--;
                        return true;
                    }
                    before = current;
                    current = current.getNext();
                }
                System.err.println("Index " + index + " is not found in the linked list.");
                return false;
            }
        }
    }

    public boolean remove(Node<T> remove) {
        Node<T> before, current;

        if (size == 0) {
            System.err.println("Can not remove from an empty list.");
            return false;
        } else {
            before = getFirst();
            current = getFirst().getNext();
            for (int count = 0; count < size; count++) {
                if (current == remove) {
                    before.setNext(current.getNext());
                    size--;
                    return true;
                }
                before = current;
                current = current.getNext();
            }
            System.err.println("Node " + remove + " is not found in the linked list.");
            return false;
        }
    }

    public Node<T> removeFirst() {
        if (head != null) {
            Node<T> temp = head;
            head = head.getNext();
            temp.setNext(null);
            size--;
            return temp;
        } else {
            System.err.println("Can not remove from an empty list.");
            return null;
        }
    }

    public Node<T> removeLast() {
        Node<T> before;
        Node<T> last;
        if (size == 0) {
            System.err.println("Can not remove from an empty list.");
            return null;
        } else {
            before = getFirst();
            for (int count = 0; count < size - 2; count++) {
                before = before.getNext();
            }
            last = tail;
            before.setNext(null);
            tail = before;
            size--;
            return last;
        }
    }

    public Node<T> get(int index) {
        Node<T> before;
        Node<T> current;
        if (size == 0) {
            System.err.println("Can not find index " + index + " in empty list.");
            return null;
        } else {
            if (index == 0) {
                return getFirst();
            } else if (index == this.size - 1) {
                return getLast();
            } else {
                before = getFirst();
                current = getFirst().getNext();
                for (int count = 0; count < index; count++) {
                    if (count == index - 1) {
                        return current;
                    }
                    before = current;
                    current = current.getNext();
                }
                System.err.println("Index " + index + " is not found in the linked list.");
                return null;
            }
        }
    }

    public Node<T> getFirst() {
        return head;
    }

    public Node<T> getLast() {
        return tail;
    }

    public long getSize() {
        return size;
    }

}