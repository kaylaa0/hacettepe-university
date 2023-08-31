# Hacettepe University
# Kayla Akyüz
# 21726914

"""
 Problem1: Quadratic Equation Solver

 It solves quadratic equation in a form of (ax^{2} + bx + c = 0)
 The constant values (a, b, and c) are provided as the comment line arguments.

"""

# Write your code here

import sys

a = int(sys.argv[1])
b = int(sys.argv[2])
c = int(sys.argv[3])

disc = (b**2) - (4 * a * c)


def oprt():
    form1 = ((disc**1 / 2) - b) / 2 * a
    if disc > 0:
        form2 = ((-1 * (disc**1 / 2)) - b) / 2 * a
    else:
        form2 = ()
    return form1, form2


if disc == 0:
    print("There is one solution")  # when discriminant value is equal to zero
elif disc > 0:
    print("There are two solutions")  # when discriminant value is greater than zero
elif disc < 0:
    print(
        "The solution is not real but complex"
    )  # when discriminant value is less than zero
form1, form2 = oprt()
if disc > 0:
    print("Solution(s): " + "{0:.2f}".format(form1) + " " + "{0:.2f}".format(form2))
elif disc == 0:
    print("Solution(s): " + "{0:.2f}".format(form1))  # if real solution exists

# End of the file
