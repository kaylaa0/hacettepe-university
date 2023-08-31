# Hacettepe University
# Kayla Akyüz
# 21726914

n = int(input())
c = 0
i = 0


def func():
    global i
    global c
    c = c + 1
    i = i + 1
    if i % 2 != 0:
        if c < n + 1:
            a = {i: [" "] * p for p in range(n - c, n - c + 1)}
            b = {i: ["*"] * p for p in range(i, i + 1)}
            print("".join(a[i]), "".join(b[i]), "".join(a[i]), end="\n")
            i = i + 1
            func()
        elif c == n + 1:
            c = c + 1
            func()
        elif c > 2 * n + 2:
            return
        elif c > n + 1:
            i = i - 3
            a = {i: [" "] * p for p in range(c - n - 3, c - n - 2)}
            b = {i: ["*"] * p for p in range(i - 3, i - 2)}
            print("".join(a[i]), "".join(b[i]), "".join(a[i]), end="\n")
            func()
    else:
        func()


func()
