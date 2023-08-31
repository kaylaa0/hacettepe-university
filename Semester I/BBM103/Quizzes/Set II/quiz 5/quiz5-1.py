# Hacettepe University
# Kayla Akyüz
# 21726914

import sys


def printer():
    n = int(sys.argv[1]) - 1
    if n == 0:
        print("*")
        quit()
    elif n <= (-1):
        print("Please enter a value bigger than 1")
        quit()
    a = (n * 2) + 1

    def printing(n, a):
        if n > 0:
            print(" " * n + "*" * (a - (2 * n)))
            n = n - 1
        elif n == 0:
            print("*" * (a))
            n = n - 1
        elif n < 0:
            if n == -((a - 1) / 2):
                print(" " * (-n) + "*" * (a + (2 * n)))
                quit()
            else:
                print(" " * (-n) + "*" * (a + (2 * n)))
                n = n - 1
        printing(n, a)

    printing(n, a)
    printer()


printer()
