/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.ArrayList;

public class q2b {
    public static ArrayList<Object> ExistingOnes = new ArrayList<Object>(0);

    public static void godeeper(ArrayList A, int depth, int exist, int pos, int current) {
        for (int i = pos; i < A.size(); i++) {
            if (A.get(i - 1) == A.get(i)) {
                if (depth + 1 == exist) {
                    ExistingOnes.add(A.get(i));
                    godeeper(A, depth + 1, exist, pos + 1, i - 1);
                } else {
                    godeeper(A, depth + 1, exist, pos + 1, i - 1);
                }
                A.remove(i);
            } else if (exist == 1) {
                ExistingOnes.add(A.get(i));
            }
        }

    }

    public static void main(String[] args) {
        // Testing
        ArrayList<Integer> A = new ArrayList<>(0);
        A.add(5);
        A.add(2);
        A.add(7);
        A.add(2);
        A.add(1);
        A.add(3);
        A.add(5);
        A.add(1);
        A.add(3);
        A.add(5);
        A.add(5);
        A.add(1);
        A.add(1);
        A.add(3);
        int n = 3;
        int exist = A.size() / n;
        int count = 0;
        while (A.size() >= exist) {
            godeeper(A, count, exist, 1, 0);
        }
        for (int i = 0; i < ExistingOnes.size(); i++) {
            System.out.println(ExistingOnes.get(i));
        }
    }
}
