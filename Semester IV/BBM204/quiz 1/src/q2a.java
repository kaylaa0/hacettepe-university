/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.ArrayList;

public class q2a {
    public static ArrayList<Object> ExistingOnes = new ArrayList<Object>(0);

    public static void combine(ArrayList A, ArrayList B, ArrayList C, boolean isSorted) {
        if (isSorted) {
            int a = 0, b = 0, c = -1;
            int A_length = A.size();
            int B_length = B.size();
            while (a < A_length && b < B_length) {
                if ((int) A.get(a) >= (int) B.get(b)) {
                    C.add(B.get(b));
                    b++;
                    c++;
                } else if ((int) A.get(a) < (int) B.get(b)) {
                    C.add(A.get(a));
                    a++;
                    c++;
                }
            }
            if (a != A_length) {
                while (a < A_length) {
                    C.add(A.get(a));
                    c++;
                    a++;
                }
            } else if (b != B_length) {
                while (b < B_length) {
                    C.add(B.get(b));
                    c++;
                    b++;
                }
            }
        } else {
            A.addAll(B);
            mergeSort(A, A.size());
            C.addAll(A);
        }
    }

    public static void mergeSort(ArrayList<Integer> a, int n) {
        if (n < 2) {
            return;
        }
        int mid = n / 2;
        ArrayList<Integer> l = new ArrayList<>(mid);
        ArrayList<Integer> r = new ArrayList<>(n - mid);

        for (int i = 0; i < mid; i++) {
            l.add(a.get(i));
        }
        for (int i = mid; i < n; i++) {
            r.add(a.get(i));
        }
        mergeSort(l, mid);
        mergeSort(r, n - mid);

        ArrayList<Integer> temp = new ArrayList<>(0);
        combine(l, r, temp, true);
        a.clear();
        a.addAll(temp);
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
        mergeSort(A, A.size());
        int n = 3;
        int exist = A.size() / n;
        if (exist == 1) {
            ExistingOnes.add(A.get(0));
        }
        for (int i = 1, times = 1; i < A.size(); i++) {
            if (A.get(i) == A.get(i - 1)) {
                times++;
            } else {
                times = 1;
            }
            if (times == exist) {
                ExistingOnes.add(A.get(i));
            }
        }
        for (int i = 0; i < ExistingOnes.size(); i++) {
            System.out.println(ExistingOnes.get(i));
        }
    }
}
