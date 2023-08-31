/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.util.ArrayList;

public class q3 {
    public static ArrayList<Object> Union = new ArrayList<Object>(0);

    public static void combine(ArrayList A, ArrayList B, ArrayList C, boolean isSorted) {
        if (isSorted) {
            int a = 0, b = 0, c = -1;
            int A_length = A.size();
            int B_length = B.size();
            while (a < A_length && b < B_length) {
                if (c == -1) {
                    if ((int) A.get(a) == (int) B.get(b)) {
                        C.add(A.get(a));
                        a++;
                        b++;
                        c++;
                    } else if ((int) A.get(a) > (int) B.get(b)) {
                        C.add(B.get(b));
                        b++;
                        c++;
                    } else if ((int) A.get(a) < (int) B.get(b)) {
                        C.add(A.get(a));
                        a++;
                        c++;
                    }
                } else {
                    if ((int) A.get(a) == (int) B.get(b)) {
                        if ((int) A.get(a) != (int) C.get(c)) {
                            C.add(A.get(a));
                            a++;
                            b++;
                            c++;
                        } else {
                            a++;
                            b++;
                        }
                    } else if ((int) A.get(a) > (int) B.get(b)) {
                        if ((int) B.get(b) != (int) C.get(c)) {
                            C.add(B.get(b));
                            b++;
                            c++;
                        } else {
                            b++;
                        }
                    } else if ((int) A.get(a) < (int) B.get(b)) {
                        if ((int) A.get(a) != (int) C.get(c)) {
                            C.add(A.get(a));
                            a++;
                            c++;
                        } else {
                            a++;
                        }
                    }
                }
            }
            if (a != A_length && b != B_length) {
                System.out.println("Something went terrible");
            } else if (a != A_length) {
                while (a < A_length) {
                    if (A.get(a) != C.get(c)) {
                        C.add(A.get(a));
                        c++;
                    }
                    a++;
                }
            } else if (b != B_length) {
                while (b < B_length) {
                    if (B.get(b) != C.get(c)) {
                        C.add(B.get(b));
                        c++;
                    }
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
        ArrayList<Integer> A = new ArrayList<>(0); // First array
        A.add(7);
        A.add(1);
        A.add(3);
        A.add(4);
        A.add(1);
        ArrayList<Integer> B = new ArrayList<>(0); // Second array
        B.add(1);
        B.add(9);
        B.add(5);
        B.add(9);
        B.add(9);
        B.add(0);
        B.add(15);
        combine(A, B, Union, false);
        for (int i = 0; i < Union.size(); i++) {
            System.out.println(Union.get(i));
        }
    }
}
