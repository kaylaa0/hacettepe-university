/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Stack<E> {
    private int top;
    private E[] data;
    private int maxSize;

    public Stack(int maxSize) {
        this.top = -1;
        this.data = (E[]) new Object[0];
        this.maxSize = maxSize;
    }

    public void Push(E elem) {
        if (!this.isFull()) {
            E[] temp = (E[]) new Object[this.data.length + 1];
            for (int i = 0; i < this.data.length; i++) {
                temp[i] = this.data[i];
            }
            temp[this.top + 1] = elem;
            this.data = temp;
            this.top += 1;
        } else {
            System.err.println("Stack is full can't add more objects.");
        }
    }

    public E Pop() {
        if (!this.isEmpty()) {
            E elem = this.data[this.top];
            this.top -= 1;
            return elem;
        } else {
            System.err.print("Can not pop from empty stack.");
            return null;
        }
    }

    public int Top() {
        return this.top;
    }

    public boolean isFull() {
        // -1 because top is array pointer size is int.
        return this.top >= this.maxSize - 1;
    }

    public boolean isEmpty() {
        if (this.top >= 0) {
            return false;
        } else {
            return true;
        }
    }

    public int Size() {
        return this.top + 1;
    }
}
