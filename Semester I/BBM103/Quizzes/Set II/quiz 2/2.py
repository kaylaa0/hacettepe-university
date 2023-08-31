# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

list = sys.argv[1].split(",")
even_numbers = []

for x in list:
    if int(x) % 2 == 0 and int(x) > 0:
        even_numbers.insert(len(even_numbers), x)
sum = 0
for x in even_numbers:
    sum = int(x) + sum
print("Even Numbers: ", str(even_numbers).replace('"', "").replace("'", ""))
print("Sum of Even Numbers: ", sum)
for x in list:
    if int(x) < 0:
        list.remove(x)
suml = 0
for x in list:
    suml = suml + int(x)
print("Even Number Rate: ", "%.3f" % (sum / suml))
