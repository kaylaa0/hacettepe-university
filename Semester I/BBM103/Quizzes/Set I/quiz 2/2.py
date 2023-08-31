# Hacettepe University
# Kayla Akyüz
# 21726914

"""
 Problem2: Even Number Evaluator

 It consider only even numbers (E) within a list of numbers (L). 
 
 The numbers in the list should be; 
 a) greater than zero, 
 b) provided in command line arguments and, 
 c) separated by commas.

 Note: This program discards negative values

"""

# Write your code here

import sys

L = [int(c) for c in sys.argv[1].split(",")]
even = []
purged = []

for n in L:
    if n != abs(n):
        continue
    if n % 2 == 0:
        even.insert(0, n)
    purged.insert(0, n)

t = 0
for m in even:
    t = t + m

pt = 0
for p in purged:
    pt = pt + p

rate = t / pt
even = list(reversed(even))
print("Even Numbers: ", end="")
print(even)  # prints the even numbers (E)
print("Sum of Even Numbers: ", end="")
print(t)  # prints the sum of even numbers
print(
    "Even Number Rate: " + "{0:.3f}".format(rate)
)  # prints the sum of even numbers divided by all numbers in the number list (L)

# End of the file
