# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

n = int(sys.argv[1]) - 1
if n <= (-1):
    print("Please enter a value bigger than 1")
    quit()
x = 1
thelist = []
for a in range(n, 0, -1):
    thelist.append(" " * a + "*" * x)
    x = x + 2
thelist.append("*" * x)
for b in range(1, n + 1):
    x = x - 2
    thelist.append(" " * b + "*" * x)
for elem in thelist:
    print(elem)
