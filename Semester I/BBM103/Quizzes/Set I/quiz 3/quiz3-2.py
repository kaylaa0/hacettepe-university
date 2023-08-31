# Hacettepe University
# Kayla Akyüz
# 21726914
import sys

rem = []
list = sys.argv[1].replace(",", " ").split()
list = [int(i) for i in list]
del list[1::2]
for i in list[1:]:
    if i in rem:
        continue
    for a in list[i - 1 :: i]:
        rem.insert(0, a)
    del list[i - 1 :: i]
for p in list:
    print(str(p) + " ", end="")
