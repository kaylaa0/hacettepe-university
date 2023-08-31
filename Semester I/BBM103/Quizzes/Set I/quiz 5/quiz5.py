# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

inputFile = open(sys.argv[1], "r")
outputFile = open(sys.argv[2], "w")
tMain = []
code = dict()
nmb = [1]
for line in inputFile:
    words = line.split()
    if int("".join(words[:1])) not in code:
        code[int("".join(words[:1]))] = {}
    code[int("".join(words[:1]))][int("".join(words[1:2]))] = words[2:]
    if words[:1] in tMain:
        continue
    else:
        tMain.insert(0, words[:1])
tMain = [[str(i)] for i in (sorted(int("".join(i)) for i in tMain))]
for dic in tMain:
    message = ("Message ", str(len(nmb)))
    outputFile.write("".join(message) + "\n")
    nmb.insert(0, 1)
    dic = "".join(dic)
    dic = int(dic)
    lcoun = []
    for tdic in code[dic]:
        if tdic in lcoun:
            continue
        else:
            lcoun.insert(len(lcoun), tdic)
    lcoun.sort()
    for i in lcoun:
        message2 = code[dic][i]
        outputFile.write(str(dic) + "\t" + str(i) + "\t" + " ".join(message2) + "\n")
