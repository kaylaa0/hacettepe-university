# Hacettepe University
# Kayla Akyüz
# 21726914
import sys

a = int(sys.argv[1])
b = int(sys.argv[2])
list = [int(x) for x in str(abs(a**b))]
if a == 0:
    print("0^b is not permitted")
else:
    print(str(a) + "^" + str(b) + " = " + str(a**b) + " = ", end="")
    for i in list[:-1]:
        print("{} + ".format(i), end="")
    print(list[-1], end="")
    if a >= 0:
        print(" = " + str(sum(list)))
    elif a <= 0:
        print(" = -" + str(sum(list)))
