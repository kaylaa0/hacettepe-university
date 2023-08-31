# Hacettepe University
# Kayla Akyüz
# 21726914

"""
 Problem3: Set Operations

 This program finds the intersection and union and difference of two sets
 (A and B) each of which corresponds to different command line argument and
 the elements of A and B should be separated by commas.

"""


# Write your code here

import sys

setA = [int(c) for c in sys.argv[1].split(",")]
prA = [str(a) for a in setA]
setB = [int(c) for c in sys.argv[2].split(",")]
prB = [str(a) for a in setB]
inte = []
inter = []
unin = []
union = []
dife = []
diffe = []


def int(x, y):
    for i in x:
        if i in y:
            inte.insert(0, i)
    inter = [str(a) for a in inte]
    return inter


def uni(x, y):
    unin = list(reversed(x))
    for i in y:
        if i in unin:
            continue
        else:
            unin.insert(0, i)
    union = [str(a) for a in unin]
    return union


def dif(x, y):
    dife = x
    for i in x:
        if i in y:
            dife.remove(i)
    diffe = [str(a) for a in dife]
    return diffe


print("Set A: ", end="")
print(prA)  # prints every element of set A
print("Set B: ", end="")
print(prB)  # prints every element of set B
print("Intersection of A and B: ", end="")
print(
    list(reversed(int(setA, setB)))
)  # print every element of intersection set of sets A and B
print("Union of A and B: ", end="")
print(
    list(reversed(uni(setA, setB)))
)  # print every element of union set of sets A and B
print("Difference of A and B: ", end="")
# Difference of A and B is print(dif(uni(setA, setB), int(setA, setB)))
# But seems like you want difference of A from B ? My math is bad ='(
print(dif(setA, setB))  #  print every element of difference set of sets A and B


# End of the file
