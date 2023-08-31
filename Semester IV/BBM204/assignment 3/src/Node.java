/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class Node {
    public String name; // Node name
    public Bag<String> adjecencyList;
    public Node parent; // When we need to store current BFS parent node
    public boolean isActive; // This is used to determine if node is visited in any situation
    public Message message; // Message containing current message
    public int capacity; // This is only used when we are dealing with graph
    public int depth; // This is only used when we are dealing with tree

    public Node(String name, int capacity) {
        this.adjecencyList = new Bag<String>();
        this.name = name;
        this.capacity = capacity;
        this.parent = null;
        this.isActive = false;
        this.message = null;
    }

    public void setActiveDeActiv() {
        this.isActive = false;
    }

    public void setActiveActiv() {
        this.isActive = true;
    }

    public void setParent(Node parent) {
        this.parent = parent;
    }

    public void setDepth(int depth) {
        this.depth = depth;
    }

    public void receiveMessage(Message message) { // Gets a message to current node
        if (this.message == null) { // If we have no message setting current message directly.
            this.message = message;
        } else {
            if (message.bestCapacity > this.message.bestCapacity) { // Otherwise checking possibilities. If new message
                                                                    // has better capacity elements just override
                                                                    // current message.
                this.message = message;
            } else if (message.bestCapacity < this.message.bestCapacity) {
                ; // If message has less capacity we ignore.
            } else if (message.bestCapacity == this.message.bestCapacity) {
                for (Bottle bottle : message.bestList) {
                    this.message.bestList.add(bottle); // If message has same capacity we add message to another.
                }
            }
        }
    }

    public void craftMessage() { // This is used to add current node capacity to message before sending it.
        if (this.message == null) {
            Bottle newBottle = new Bottle(this.name, this.capacity);
            Message newMessage = new Message();
            newMessage.bestList.add(newBottle);
            newMessage.bestCapacity = this.capacity;
            this.message = newMessage;
        } else {
            if (this.capacity > this.message.bestCapacity) { // According to possibilities either we ignore current
                                                             // node's capacity or add it to messsage.
                Bottle newBottle = new Bottle(this.name, this.capacity);
                Message newMessage = new Message();
                newMessage.bestList.add(newBottle);
                newMessage.bestCapacity = this.capacity;
                this.message = newMessage;
            } else if (this.capacity < this.message.bestCapacity) {
                ;
            } else if (this.capacity == this.message.bestCapacity) {
                Bottle newBottle = new Bottle(this.name, this.capacity);
                this.message.bestList.add(newBottle);
            }
        }
    }

    public Message getMessage() {
        return message;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder(this.name);
        stringBuilder.append("(").append(String.valueOf(this.capacity)).append(")-->");
        for (String neighbor : this.adjecencyList) {
            stringBuilder.append(neighbor).append(" ");
        }
        stringBuilder.delete(stringBuilder.length() - 1, stringBuilder.length());
        stringBuilder.append("\n");
        return stringBuilder.toString();
    }
}
