# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

if len(sys.argv[1:]) != 2:
    print("Please run with 2 lists")
    quit()
listA = sys.argv[1].split(",")
listB = sys.argv[2].split(",")
print("Set A: ", listA)
print("Set B: ", listB)
inter = []
for x in listA:
    if x in listB:
        inter.insert(len(inter), x)
differ = []
listU = []
for x in inter:
    listB.remove(x)
for x in listA:
    listU.insert(len(listU), x)
for x in listB:
    listU.insert(len(listU), x)
for x in inter:
    listA.remove(x)

print("Intersection of A and B: ", inter)
print("Union of A and B: ", listU)
print("Difference of A and B: ", listA)
