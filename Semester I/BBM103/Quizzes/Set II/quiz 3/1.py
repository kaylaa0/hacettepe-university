# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

dataFile = open("WBC.data", "r").read()
dataDic = {i.split(",")[0]: i.split(",")[1:] for i in dataFile.split("\n")}
totalavarages = []
inputlist = []

for x in str(sys.argv[1]).split(","):
    inputlist.append(x)


def funDataClean():
    for key, value in dataDic.items():
        if "?" in value:
            column = value.index("?")
            current = []
            for a in dataDic.values():
                if a[-1] == value[-1] and a[column] != "?":
                    current.append(int(a[column]))
            avarage = round(sum(current) / len(current))
            totalavarages.append(avarage)
            dataDic[key][column] = avarage


funDataClean()

totalavarage = sum(totalavarages) / len(totalavarages)
print("The average of all missing values is  : " + "{0:.4f}".format(totalavarage))
printdic = dataDic.copy()


def performStepWiseSearch():
    for column, input in enumerate(inputlist):
        if input == "?":
            continue
        else:
            input = input.split(":")
            if "<=" == input[0]:
                for key, value in dataDic.items():
                    if value[column] > int(input[1]):
                        printdic.pop(key, None)
            elif "!=" == input[0]:
                for key, value in dataDic.items():
                    if int(value[column]) == int(input[1]):
                        printdic.pop(key, None)
            elif ">=" == input[0]:
                for key, value in dataDic.items():
                    if int(value[column]) < int(input[1]):
                        printdic.pop(key, None)
            elif "<" == input[0]:
                for key, value in dataDic.items():
                    if int(value[column]) >= int(input[1]):
                        printdic.pop(key, None)
            elif ">" == input[0]:
                for key, value in dataDic.items():
                    if int(value[column]) <= int(input[1]):
                        printdic.pop(key, None)
            elif "=" == input[0]:
                for key, value in dataDic.items():
                    if int(value[column]) != int(input[1]):
                        printdic.pop(key, None)
            else:
                print(
                    """

Please only use operators in given list:

1 < less than
2 <= less than or equal to
3 > greater than
4 >= greater than or equal to
5 != not equal to
6 = equal to
7 ? any value                
                     
                        """
                )


performStepWiseSearch()
malignantcase = []
benigncase = []
for case in printdic.values():
    if case[-1] == "malignant":
        malignantcase.append(case)
    elif case[-1] == "benign":
        benigncase.append(case)
probablity = len(malignantcase) / len(printdic)

print(
    "\nTest Results:\n"
    "----------------------------------------------"
    "\nPositive (malignant) cases            : "
    + str(len(malignantcase))
    + "\nNegative (benign) cases               : "
    + str(len(benigncase))
    + "\nThe probability of being positive     : "
    + "{0:.4f}".format(probablity)
    + "\n----------------------------------------------"
)
