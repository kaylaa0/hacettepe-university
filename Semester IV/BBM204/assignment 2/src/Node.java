/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Node<T> {
    // Basic Node class of my linked list class.
    private T data;
    private Node<T> next;

    public Node() {
        this.data = null;
        this.next = null;
    }

    public Node(T data, Node<T> next) {
        this.data = data;
        this.next = next;
    }

    public T getData() {
        return data;
    }

    public void setData(T data) {
        this.data = data;
    }

    public Node<T> getNext() {
        return next;
    }

    public void setNext(Node<T> next) {
        this.next = next;
    }

    // This method is overridden to ease debugging.
    @Override
    public String toString() {
        return this.data.toString();
    }
}
