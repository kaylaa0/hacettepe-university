# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

if len(sys.argv[1:]) != 3:
    print("Please run with 3 arguments")
    quit()
a, b, c = int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])
disc = (b**2) - (4 * a * c)
if disc > 0:
    print("There are two solutions")
    print(
        "Solution(s): ",
        "%.2f" % (((-b) + (disc ** (1 / 2))) / (2 * a)),
        "%.2f" % (((-b) - (disc ** (1 / 2))) / (2 * a)),
    )
    quit()
elif disc < 0:
    print("The solution is not real")
    quit()
elif disc == 0:
    print("There is one solution")
    print("Solution(s): ", "%.2f" % (((-b) + (disc ** (1 / 2))) / (2 * a)))
    quit()
