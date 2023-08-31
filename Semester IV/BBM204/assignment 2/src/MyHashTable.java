/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

public class MyHashTable {
    final int DOUBLE_HASH_MAX_SEARCH_DEPTH = 5000; // How long will double hash function search for a valid hash value.
                                                   // 5000 is tested to be not crashing.
    final int TABLE_SIZE; // Hash table size, as requested by assigment paper.
    /*
     * I wanted to use inheritance however assigment paper stated we must have only
     * one MyHashTable class,
     * so I use this method where every hash table stores its structural search
     * method mode.
     */
    final int SEARCH_MODE; // 1 is for Separate Chaining, 2 is for Linear Probing and 3 is for Double
                           // Hashing.
    int comparisionCounter; // Comparision printed at output, counted to here.
    long startTime; // When running get() method nanoTime() is stored here.
    public LinkedList<Employee>[] EmployeeData; // Named and shaped as requested in paper, EmployeeData is a [] of
                                                // LinkedList.
    /*
     * Even for linear probing or double hashing it is [] of LinkedList. Because it
     * is stated as such in the assigment paper.
     * How I made it work is they become a linked list of one node (head node).
     */

    public MyHashTable(int list_size_n, double alpha, int mode) {
        this.TABLE_SIZE = (int) Math.floor(list_size_n / alpha); // Calculated size of hash table as stated in assigment
                                                                 // paper.
        this.EmployeeData = new LinkedList[this.TABLE_SIZE];
        if (mode > 0 && mode < 4) {
            this.SEARCH_MODE = mode;
        } else {
            this.SEARCH_MODE = 0; // If mode is invalid it will be set to 0. You can not use methods.
        }
    }

    public void put(int key, Employee employee) {
        /*
         * put() and get() firstly check if mode is valid.
         */
        if (this.SEARCH_MODE == 1) {
            if (EmployeeData[hash(key)] == null) {
                // If that place is available we occupy it.
                EmployeeData[hash(key)] = new LinkedList();
                EmployeeData[hash(key)].addHead(new Node(employee, null));
                return;
            } else {
                // If not, that means it is occupied, which means there already exist a linked
                // list there.
                // For separate chaining we add next to tail of the linked list, meaning and of
                // the linked list.
                EmployeeData[hash(key)].addNext(EmployeeData[hash(key)].getLast(), new Node(employee, null));
                return;
            }
        } else if (this.SEARCH_MODE == 2) {
            if (EmployeeData[hash(key)] == null) {
                // If that place is available we occupy it.
                EmployeeData[hash(key)] = new LinkedList();
                EmployeeData[hash(key)].addHead(new Node(employee, null));
                return;
            } else {
                // If not, that means it is occupied, which means there already exist a linked
                // list there.
                for (int i = 1; i <= this.TABLE_SIZE; i++) {
                    // For linear probing we check in the hash table for increasing order.
                    try {
                        if (EmployeeData[hash(key) + i] == null) {
                            // If found a spot we occupy it.
                            EmployeeData[hash(key) + i] = new LinkedList();
                            EmployeeData[hash(key) + i].addHead(new Node(employee, null));
                            return;
                        } else if (((Integer) (hash(key) + i)).equals(this.TABLE_SIZE - 1)) {
                            // We break if we hit the last item of the list, search will continue looping
                            // from beginning.
                            break;
                        }
                    } catch (ArrayIndexOutOfBoundsException e) {
                        break;
                    }
                }
                for (int i = 0; i <= hash(key); i++) {
                    // Starting from beginning we check until hash key, if still not found the list
                    // is full.
                    if (EmployeeData[i] == null) {
                        // If found a spot we occupy it.
                        EmployeeData[i] = new LinkedList();
                        EmployeeData[i].addHead(new Node(employee, null));
                        return;
                    }
                }
                System.err.println("No free spots found in linear probing hash table.");
            }
        } else if (this.SEARCH_MODE == 3) {
            if (EmployeeData[hash(key)] == null) {
                // If that place is available we occupy it.
                EmployeeData[hash(key)] = new LinkedList();
                EmployeeData[hash(key)].addHead(new Node(employee, null));
            } else {
                // If not we calculate double hash key, the method already returns the available
                // key by checking spots and increasing the "i" via recursion. However it will
                // stop at the DOUBLE_HASH_MAX_SEARCH_DEPTH.
                int dkey = getDoubleHash(key, 1);
                if (((Integer) dkey).equals(-55)) {
                    System.err.println("There was no free spots to put " + key + " in Double Hashing hash table");
                } else {
                    EmployeeData[dkey] = new LinkedList();
                    EmployeeData[dkey].addHead(new Node(employee, null));
                }
            }
        }
    }

    public Employee get(int key) {
        /*
         * put() and get() firstly check if mode is valid.
         */
        this.comparisionCounter = 0; // Comparision counter is set to 0 at the beginning of get method.
        if (this.SEARCH_MODE == 1) {
            this.startTime = System.nanoTime(); // Start time of the method is set.
            if (EmployeeData[hash(key)] == null) {
                this.comparisionCounter++;
                // If given hash key is not even occupied.
                System.err.println("Object is not found. (Linked list does not exist)");
                return null;
            } else {
                // If not we search only the remaining nodes of linked list for the key.
                for (int i = 0; i < EmployeeData[hash(key)].getSize(); i++) {
                    this.comparisionCounter++; // Increased every check.
                    if (EmployeeData[hash(key)].get(i) != null) {
                        Integer check = EmployeeData[hash(key)].get(i).getData().getPhone();
                        if (check.equals(key)) {
                            // If match is found we return data.
                            return EmployeeData[hash(key)].get(i).getData();
                        }
                    }
                }
                // If match is not found the key does not exist.
                System.err.println("Object is not found. (Object does not exist in linked list)");
                return null;
            }
        } else if (this.SEARCH_MODE == 2) {
            this.comparisionCounter++;
            this.startTime = System.nanoTime(); // Start time of the method is set.
            if (EmployeeData[hash(key)] == null) {
                // If given hash key is not even occupied.
                System.err.println("Object is not found.");
                return null;
            } else {
                Integer check = EmployeeData[hash(key)].getFirst().getData().getPhone();
                if (check.equals(key)) {
                    // First of all checking the key, if it is the one we are searching for.
                    return EmployeeData[hash(key)].getFirst().getData();
                } else {
                    // If not we start looking for it in the hash table.
                    for (int i = 1; i <= this.TABLE_SIZE; i++) {
                        // First looking from key to end of the table.
                        this.comparisionCounter++;
                        try {
                            if (EmployeeData[hash(key) + i] == null) {
                                System.err.println(
                                        "Object is not found. (Object does not exist in hash table, Linear Probing, hit null value while moving toward end of the list.)");
                                return null;
                            }
                            Integer check2 = EmployeeData[hash(key) + i].getFirst().getData().getPhone();
                            if (check2.equals(key)) {
                                return EmployeeData[hash(key) + i].getFirst().getData();
                            } else if (((Integer) (hash(key) + i)).equals(this.TABLE_SIZE - 1)) {
                                // End of the list, will continue search from the beginning of the list by
                                // looping.
                                break;
                            }
                        } catch (ArrayIndexOutOfBoundsException e) {
                            // This only occurs if the current key is the last key of hashtable, when we
                            // trying to look 1 forward, it will give this error.
                            break;
                        }
                    }
                    for (int i = 0; i < hash(key); i++) {
                        // Starting from beginning we look until the key, if not found it does not exist
                        // for sure.
                        this.comparisionCounter++;
                        if (EmployeeData[i] == null) {
                            System.err.println(
                                    "Object is not found. (Object does not exist in hash table, Linear Probing, hit null value after looped from beginning.)");
                            return null;
                        }
                        Integer check2 = EmployeeData[i].getFirst().getData().getPhone();
                        if (check2.equals(key)) {
                            return EmployeeData[i].getFirst().getData();
                        }
                    }
                    System.err.println("Object is not found. (Object does not exist in hash table, Linear Probing)");
                    return null;
                }
            }
        } else if (this.SEARCH_MODE == 3) {
            this.comparisionCounter++;
            this.startTime = System.nanoTime(); // Start time of the method is set.
            if (EmployeeData[hash(key)] == null) {
                // If given hash key is not even occupied.
                System.err.println("Object is not found.");
                return null;
            } else {
                Integer check = EmployeeData[hash(key)].getFirst().getData().getPhone();
                if (check.equals(key)) {
                    // First of all checking the key, if it is the one we are searching for.
                    return EmployeeData[hash(key)].getFirst().getData();
                } else {
                    for (int i = 1; i <= DOUBLE_HASH_MAX_SEARCH_DEPTH; i++) {
                        // For maximum of double hash generated depth we look for, anything beyond that
                        // we didn't even put the key in.
                        this.comparisionCounter++;
                        if (EmployeeData[doubleHashFunction(key, i)] == null) {
                            System.err.println(
                                    "Object is not found. (Object does not exist in hash table, Double Hashing, hit null value at search depth "
                                            + i + ")");
                            return null;
                        }
                        Integer check2 = EmployeeData[doubleHashFunction(key, i)].getFirst().getData().getPhone();
                        if (check2.equals(key)) {
                            // If match is found.
                            return EmployeeData[doubleHashFunction(key, i)].getFirst().getData();
                        }
                    }
                    System.err.println(
                            "Object is not found. (Object does not exist in hash table, Double Hashing search depth "
                                    + this.DOUBLE_HASH_MAX_SEARCH_DEPTH + ")");
                    return null;
                }
            }
        } else {
            System.err.println("Hash table does not have a search mode configured.");
            return null;
        }
    }

    private int hash(int key) {
        // The function given and desired by assigment paper. Why this function ?
        // Because it is required in assignment paper. At first I thought I could use
        // this by calling double hash function here but it was unnecessary.
        return hash1(key);
    }

    private int hash1(int key) {
        // The function given and desired by assigment paper. This is the regular/base
        // hash function.
        return (key % TABLE_SIZE);
    }

    private int doubleHashFunction(int key, int i) {
        // This method overload is implemented by me, just to reach desired deepness in
        // doubleHashFunction. This method is used while we are searching for the key.
        return (hash1(key) + (i * doubleHashFunction(key))) % TABLE_SIZE;
    }

    private int getDoubleHash(int key, int i) {
        // The function given and desired by assigment paper. Automatically gets
        // available hash value via recursion.
        if (EmployeeData[(hash1(key) + (i * doubleHashFunction(key))) % TABLE_SIZE] == null) {
            return (hash1(key) + (i * doubleHashFunction(key))) % TABLE_SIZE;
        } else if (((Integer) i).equals(this.DOUBLE_HASH_MAX_SEARCH_DEPTH)) {
            System.err.println("Couldn't find free space in the list, Double Hashing search depth "
                    + this.DOUBLE_HASH_MAX_SEARCH_DEPTH + ")");
            return -55;
        } else {
            return getDoubleHash(key, i + 1);
        }
    }

    private int doubleHashFunction(int key) {
        // The function given and desired by assigment paper. The algorithm as stated in
        // paper.
        return 1 + (key % (TABLE_SIZE - 1));
    }

    public long[] analyseSearch(int key) {
        // While analyzing in order to get exact result we warm up JVM by running method
        // lots of time and run couple more times and get average.
        /*
         * !
         * ! If you are seeing any performance issues or space issues it must be related
         * to what is happening here, try to lower values for losing test accuracy
         * !
         */
        this.loadRunLots(key, 100000);
        int testAmount = 30000;
        long testResults[] = new long[testAmount];
        for (int i = 0; i < testAmount; i++) {
            // Creating a test Employee at testAmount times and storing nanoTime() result.
            Employee test = this.get(key);
            long endTime = System.nanoTime();
            testResults[i] = endTime - this.startTime;
        }
        // Calling function to calculate average of the test list.
        this.getAvarage(testResults, testAmount);
        // The average runtime and total comparision number is returned.
        return new long[] { testResults[0], this.comparisionCounter };
    }

    private void loadRunLots(int key, int times) {
        // Runs get() method for desired times in order to warm up JVM.
        for (int i = 0; i < times; i++) {
            this.get(key);
        }
    }

    private void getAvarage(long[] list, int len) {
        // Every nth long is added and divided by 2 with the nth long from end of the
        // list, while dividing list by 2. This way at last we reach average of the
        // list.
        for (int x = len / 2; x >= 1; x = x / 2) {
            for (int a = 0; a < x; a++) {
                list[a] = (list[a] + list[a + x]) / 2;
            }
        }
    }
}
