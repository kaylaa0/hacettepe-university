# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

from operator import itemgetter

input = open(sys.argv[1], "r")
open(sys.argv[2], "w").close()

sys.stdout = open(sys.argv[2], "r+")
thetext = {}

for line in input:
    value = {}
    line = line.strip("\n")
    line = line.split("\t")
    value = (int(line[1]), line[2])
    thetext.setdefault(int(line[0]), [])
    thetext[int(line[0])].append(value)
counter = 1
for k in sorted(thetext.keys()):
    print("Message " + str(counter))
    counter = counter + 1
    for v in sorted(thetext[k], key=itemgetter(0)):
        print(str(k) + "\t" + str(v[0]) + "\t" + str(v[1]))
sys.stdout = (
    sys.__stdout__
)  # This part of the code is copied from my own assignment 3 project.
output = open(sys.argv[2], "r")
try:
    a = output.read()[-1]
except IndexError:
    print("Output file is empty or corrupted. Nothing to print.")
    sys.exit()
output = open(sys.argv[2], "r")
if a == "\n":
    print(output.read()[:-1])
else:
    print(output.read())
output.close()
