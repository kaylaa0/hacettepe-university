# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

theword = [str(i) for i in sys.argv[1]]
theletters = [str(i) for i in sys.argv[2]]
for lett in theletters:
    if lett == ",":
        theletters.remove(",")

revel = []
revelp = []
ing = []
outg = []
nonword = [
    "q",
    "w",
    "e",
    "r",
    "t",
    "y",
    "u",
    "o",
    "p",
    "a",
    "s",
    "d",
    "f",
    "g",
    "h",
    "j",
    "k",
    "l",
    "i",
    "z",
    "x",
    "c",
    "v",
    "b",
    "n",
    "m",
    "ı",
    "ü",
    "ğ",
    "ç",
    "ö",
]
hp = [0, 0, 0, 0, 0]
tictoc = []
opon = []
gec = []

for everyeach in theword:
    opon.insert(len(opon), "-")

for letter in theword:
    if letter in nonword:
        nonword.remove(letter)

print("You have " + str(len(hp)) + "  guesses left")
print(opon)

for a in theletters:
    if len(hp) <= 0:
        break
    gec.clear()

    def inm(x):
        if x in ing:
            hp.remove(0)
            tictoc.insert(0, 1)
            gec.insert(0, 3)
        elif x in theword:
            revel.insert(len(revel), x)
            ing.insert(len(ing), x)
        elif x in nonword:
            hp.remove(0)
            tictoc.insert(0, 1)
            gec.insert(0, 1)

    def outm(y):
        if y in outg:
            hp.remove(0)
        elif y not in nonword:
            hp.remove(0)
        elif y in nonword:
            outg.insert(len(outg), y)
            tictoc.remove(1)
            gec.insert(0, 2)

    if len(tictoc) == 0:
        inm(a)
        mode = "IN"
    else:
        outm(a)
        mode = "OUT"
    revelp = []
    for i in theword:
        if i in revel:
            revelp.insert(len(revelp), str(i))
        else:
            revelp.insert(len(revelp), "-")
    if "-" not in revelp:
        print("--------------------------------------------")
        print("Guessed word: " + a + " You are in " + mode + " mode")
        print("You have " + str(len(hp)) + "  guesses left")
        print(revelp)
        break
    if len(hp) > 0:
        if len(gec) == 0:
            print("--------------------------------------------")
            print("Guessed word: " + a + " You are in " + mode + " mode")
            print("You have " + str(len(hp)) + "  guesses left")
            print(revelp)
        elif 1 in gec:
            print("--------------------------------------------")
            print("Guessed word: " + a + " The game turned into " + "OUT" + " mode")
            print("You have " + str(len(hp)) + "  guesses left")
            print(revelp)
        elif 2 in gec:
            print("--------------------------------------------")
            print("Guessed word: " + a + " The game turned into " + "IN" + " mode")
            print("You have " + str(len(hp)) + "  guesses left")
            print(revelp)
        elif 3 in gec:
            print("--------------------------------------------")
            print(
                "Guessed word: "
                + a
                + " is used in IN mode. The game turned into "
                + "OUT"
                + " mode"
            )
            print("You have " + str(len(hp)) + "  guesses left")
            print(revelp)
if len(hp) > 0:
    if "-" not in revelp:
        print(
            """--------------------------------------------
    
You won the game"""
        )
    else:
        print(
            """--------------------------------------------
        
You finished all letters
You lost the game
"""
        )
elif len(hp) <= 0:
    print(
        """
You lost the game"""
    )
