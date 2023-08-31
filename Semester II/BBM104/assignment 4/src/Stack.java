/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;

public class Stack<E> {
    private int top; // My approach for stack and queue using basic array.
    private E[] data;

    public Stack() {
        this.top = -1;
        this.data = (E[]) new Object[0];
    }

    public int getSize() {
        return this.top + 1;
    }

    public boolean isEmpty() {
        if (this.top >= 0) {
            return false;
        } else {
            return true;
        }
    }

    public E pop() {
        if (!this.isEmpty()) {
            E elem = this.data[this.top];
            this.top -= 1;
            return elem;
        } else {
            System.err.print("Can not pop from empty stack.");
            return null;
        }
    }

    public void push(E elem) {
        E[] temp = (E[]) new Object[this.data.length + 1];
        for (int i = 0; i < this.data.length; i++) {
            temp[i] = this.data[i];
        }
        temp[this.top + 1] = elem;
        this.data = temp;
        this.top += 1;
    }

    public E peek() {
        if (!this.isEmpty())
            return this.data[this.top];
        else
            return null;
    }

    public void print() {
        System.out.println(this.toString());
    }

    public void save(String filename) {
        try {
            Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(new File(filename))));
            writer.write(this.toString());
            writer.close();
        } catch (IOException e) {
            System.err.println("Problem writing stack to " + filename + ".");
            e.printStackTrace();
        }
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        for (int i = this.top; i > -1; i--) {
            builder.append(this.data[i]).append(" ");
        }
        builder.delete(builder.length() - 1, builder.length());
        return builder.toString();
    }
}
