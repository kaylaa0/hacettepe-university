# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

word_list = sys.argv[1:]
print()
if len(sys.argv[1:]) != 3:
    print("Please run with 3 arguments")
    quit()
    exit()
for x in word_list:
    if x.isalpha() is False:
        print("Argument ", x, " is not a word. All arguments should be word")
        quit()
        exit()
point_list = []
for x in word_list:
    point_list.insert(0, len(x))
if len(point_list) != len(set(point_list)):
    print("Arguments should be a different length")
    quit()
    exit()
letter_list = sorted(
    list(
        str(sys.argv[1:])
        .replace("[", "")
        .replace("]", "")
        .replace(",", "")
        .replace('"', "")
        .replace("'", "")
        .replace(" ", "")
    )
)
print("Find longest word using letters given below", "\n")
print(str(letter_list).replace("[", "").replace("]", ""), "\n")
guess = input("Guess a longest word: ")
print()
if guess not in word_list:
    print("The word you guessed is not in the list", "\n")
    print("You lost !")
    quit()
    exit()
else:
    print("You found a word from list", "\n")
    if len(guess) is max(point_list):
        print("You won 50 points !")
    elif len(guess) is min(point_list):
        print("You won 10 points !")
    else:
        print("You won 30 points !")
