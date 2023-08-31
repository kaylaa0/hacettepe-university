/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;

public class Queue<E> {
    private int front, rear;
    private E[] data;

    public Queue() {
        this.front = -1;
        this.rear = -1;
        this.data = (E[]) new Object[0];
    }

    public Queue(Queue<E> queue) {
        if (queue.isEmpty()) {
            this.front = -1;
            this.rear = -1;
            this.data = (E[]) new Object[0];
        } else {
            this.front = 0;
        }
    }

    public int getSize() {
        return this.rear - this.front + 1;
    }

    public boolean isEmpty() {
        if (this.front == -1 && this.rear == -1) {
            return true;
        } else if (this.front > this.rear) {
            return true;
        } else {
            return false;
        }
    }

    public E deQueue() { // More method names for same operation. I couldn't decide official names for
                         // these methods so both work.
        return this.remove();
    }

    public void enQueue(E elem) {
        this.add(elem);
    }

    public void add(E elem) {
        if (this.front == -1) {
            this.front = 0;
        }
        E[] temp = (E[]) new Object[this.data.length + 1];
        for (int i = 0; i < this.data.length; i++) {
            temp[i] = this.data[i];
        }
        this.rear += 1;
        temp[this.rear] = elem;
        this.data = temp;
    }

    public E remove() {
        if (!this.isEmpty()) {
            E elem = this.data[front];
            this.front += 1;
            if (this.isEmpty()) { // In order to preserve memory when the queue gets emptied we flush for new
                                  // queue. I also thought about doing it when front
                this.rear = -1;
                this.front = -1;
                this.data = (E[]) new Object[0];
            }
            return elem;
        } else {
            System.err.println("Can not remove from empty queue.");
            return null;
        }
    }

    public E peek() {
        if (!this.isEmpty())
            return this.data[this.front];
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
        for (int i = this.front; i < this.rear + 1; i++) {
            builder.append(this.data[i]).append(" ");
        }
        builder.delete(builder.length() - 1, builder.length());
        return builder.toString();
    }
}
