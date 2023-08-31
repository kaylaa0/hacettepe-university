
/*
 Hacettepe University
 Kayla Akyüz
 21726914

 Answer:
 T(n)=3T(n/3)+ O(n) = O(nlogn) with base 3.
 Hence doesn't change.

*/
import java.util.ArrayList;

public class q4 {
    public static void combine(ArrayList A, ArrayList B, ArrayList C, ArrayList D) {
        int a = 0, b = 0, c = 0;
        int A_length = A.size();
        int B_length = B.size();
        int C_length = C.size();
        while (a < A_length && b < B_length && c < C_length) {
            if ((int) A.get(a) >= (int) B.get(b)) {
                if ((int) B.get(b) >= (int) C.get(c)) {
                    D.add(C.get(c));
                    c++;
                } else if ((int) B.get(b) < (int) C.get(c)) {
                    D.add(B.get(b));
                    b++;
                }
            } else if ((int) A.get(a) < (int) B.get(b)) {
                if ((int) A.get(a) >= (int) C.get(c)) {
                    D.add(C.get(c));
                    c++;
                } else if ((int) A.get(a) < (int) C.get(c)) {
                    D.add(A.get(a));
                    a++;
                }
            }
        }
        while (a < A_length && b < B_length) {
            if ((int) A.get(a) >= (int) B.get(b)) {
                D.add(B.get(b));
                b++;
            } else if ((int) A.get(a) < (int) B.get(b)) {
                D.add(A.get(a));
                a++;
            }
        }
        while (c < C_length && b < B_length) {
            if ((int) C.get(c) >= (int) B.get(b)) {
                D.add(B.get(b));
                b++;
            } else if ((int) C.get(c) < (int) B.get(b)) {
                D.add(C.get(c));
                c++;
            }
        }
        while (a < A_length && c < C_length) {
            if ((int) A.get(a) >= (int) C.get(c)) {
                D.add(C.get(c));
                c++;
            } else if ((int) A.get(a) < (int) C.get(c)) {
                D.add(A.get(a));
                a++;
            }
        }
        while (a < A_length) {
            D.add(A.get(a));
            a++;
        }
        while (b < B_length) {
            D.add(B.get(b));
            b++;
        }
        while (c < C_length) {
            D.add(C.get(c));
            c++;
        }
    }

    public static void mergeSort(ArrayList<Integer> a, int n) {
        if (n < 2) {
            return;
        }
        int mid1 = n / 3;
        int mid2 = 2 * (n / 3) + 1;
        ArrayList<Integer> l = new ArrayList<>(mid1);
        ArrayList<Integer> m = new ArrayList<>(mid2 - mid1);
        ArrayList<Integer> r = new ArrayList<Integer>(n - mid2);

        for (int i = 0; i < mid1; i++) {
            l.add(a.get(i));
        }
        for (int i = mid1; i < mid2; i++) {
            m.add(a.get(i));
        }
        for (int i = mid2; i < n; i++) {
            r.add(a.get(i));
        }
        mergeSort(l, mid1);
        mergeSort(m, mid2 - mid1);
        mergeSort(r, n - mid2);

        ArrayList<Integer> temp = new ArrayList<>(0);
        combine(l, m, r, temp);
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
        for (int i = 0; i < A.size(); i++) {
            System.out.println(A.get(i));
        }
    }
}
