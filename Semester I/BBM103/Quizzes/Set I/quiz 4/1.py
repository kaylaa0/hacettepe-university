# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

# My First Data Miner Program

"""
This program
a) performs data cleaning to remove missing values existing on the database.
b) calculates probability of being breast cancer of an imaginary patient
   by inspecting her test results provided as command-line argument.
"""

# Starter code that reads database named bws.data and loads it to a dictionary 'dataDic'

dataFile = open("WBC.data", "r").read()
dataDic = {i.split(",")[0]: i.split(",")[1:] for i in dataFile.split("\n")}

# Do not alter any upper lines so that you do not get trouble in loading data file

# This function modifies data content though it is a void function due to the mutability of dictionary object


def funDataClean(dataDic, recordID, attributeIndex, classLabel, meanMissingVal):
    attrVal = [
        int(i[attributeIndex])
        for i in dataDic.values()
        if i[-1] == classLabel and i[attributeIndex] != "?"
    ]
    meanAttrVal = round(sum(attrVal) / len(attrVal))
    meanMissingVal.append(meanAttrVal)
    dataDic[recordID][attributeIndex] = meanAttrVal


def performStepWiseSearch(IDs, dataDic, attrIndex, relOp, conVal):
    if relOp == "<":
        ID2remove = [
            i for i in recordIDs2Display if int(dataDic[i][attrIndex]) >= conVal
        ]
    elif relOp == "<=":
        ID2remove = [
            i for i in recordIDs2Display if int(dataDic[i][attrIndex]) > conVal
        ]
    elif relOp == ">":
        ID2remove = [
            i for i in recordIDs2Display if int(dataDic[i][attrIndex]) <= conVal
        ]
    elif relOp == ">=":
        ID2remove = [
            i for i in recordIDs2Display if int(dataDic[i][attrIndex]) < conVal
        ]
    elif relOp == "=":
        ID2remove = [
            i for i in recordIDs2Display if int(dataDic[i][attrIndex]) != conVal
        ]
    elif relOp == "!=":
        ID2remove = [
            i for i in recordIDs2Display if int(dataDic[i][attrIndex]) == conVal
        ]
    else:
        print("Unsupported type of relational operator! Search ignored")
        ID2remove = []

    for i in ID2remove:
        IDs.remove(i)

    return IDs


# 1st phase: Cleaning WBC Database

meanMissingVal = []

for key, val in dataDic.items():
    if "?" in val:
        funDataClean(dataDic, key, val.index("?"), val[-1], meanMissingVal)

print(
    "The average of all missing values is  : "
    + "{0:.4f}".format(sum(meanMissingVal) / len(meanMissingVal))
)

# 2nd phase: Retrieving knowledge from WBC dataset

recordIDs2Display = [i for i in dataDic.keys()]

conditions = [i for i in str(sys.argv[1]).split(",")]

for i in conditions:
    if i != "?":
        recordIDs2Display = performStepWiseSearch(
            recordIDs2Display.copy(),
            dataDic,
            conditions.index(i),
            i.split(":")[0],
            int(i.split(":")[1]),
        )


numberClass1 = len(
    [i for i in recordIDs2Display if dataDic[i][-1] == "benign"]
)  # number of benign cases
numberClass2 = len(
    [i for i in recordIDs2Display if dataDic[i][-1] == "malignant"]
)  # number of malignant cases

print(
    "\nTest Results:\n"
    "----------------------------------------------"
    "\nPositive (malignant) cases            : "
    + str(numberClass2)
    + "\nNegative (benign) cases               : "
    + str(numberClass1)
    + "\nThe probability of being positive     : "
    + "{0:.4f}".format(numberClass2 / len(recordIDs2Display))
    + "\n----------------------------------------------"
)
