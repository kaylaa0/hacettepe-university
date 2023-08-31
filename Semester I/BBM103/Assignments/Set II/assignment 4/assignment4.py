# Hacettepe University
# Kayla Akyüz
# 21726914

#  Notes:
#       - This game provides a game over detection system.
#       - To break out of game simply write stop or exit or quit. Letter case independent.
#       - To see all available moves write show or moves or helpme. Letter case independent.
#       - To see all available command write help. Letter case independent
#       - Your input must be like 'x y' where x and y are integers and separated by one space. Otherwise you will be asked for input again and again.
#       - You will not receive error message except if your selection is in game's range but empty. You will just be asked for input again but the game does detect all type of errors separately, it just doesn't give errors because any change in output format might effect my grade.

import sys  # Importing necessary modules

input_file = open(sys.argv[1], "r")  # Reading input file.

cosystem = {}  # Creating an empty dic for our coordinate system.
nesystem = {}  # Creating an empty dic for our neighbourhood system.

maxx = 0  # Setting maximum column number to 0.
maxy = 0  # Setting maximum row number to 0.

gameover = False  # Setting game over status to False.
gameon = True
score = 0  # Setting score to 0.


def coordinatecreator(
    input,
):  # This function creates an coordinated cell system for the game.
    linecounter = 0  # Setting line counter to 0.
    for line in input:  # Reading input file line by line.
        if line == " " or line == "\n":  # If line is empty skipping it.
            continue
        else:
            linecounter = (
                linecounter + 1
            )  # Increasing line counter by one. Now we are in the line number linecounter.
            elemcounter = 0  # Setting row counter to 0.
            for elem in line:  # Reading line character by character.
                if (
                    elem == " " or elem == "\n"
                ):  # Skipping if current character is space or ending.
                    continue
                elemcounter = (
                    elemcounter + 1
                )  # Increasing row counter by one. Now we are in the row number elemcounter.
                cosystem[elemcounter, linecounter] = int(
                    elem
                )  # Inserting our character by the keys of current (column, row) to our coordinate system dic.
            global maxx
            maxx = elemcounter  # Setting our maximum column number to current column number.
    global maxy
    maxy = linecounter  # Setting our maximum row number to current row number.


coordinatecreator(input_file)  # Running coordinate creator function for input file.


def neighbourhoodcreator():  # This function detects the all neighbourhood systems in game and attaches them to all of their contents.
    global nesystem

    def move(
        deads, current, value, rope, before
    ):  # This is our main function for neighbourhood creator. Basically it starts from given cell and goes to every direction until it can't go. Conditions are; cell must not be a previous cell that we are gone while reaching the current cell, cell must not be a previously reached dead end cell, cell must have same value as our starting cell, cell must be an existing cell in our coordinate system. When we reach a dead end cell that we can't move anymore we add it to dead end cell list.
        x, y = current  # Dividing current coordinates to its components.
        if (x - 1, y) in cosystem:  # Trying to move left.
            if (
                value == cosystem[x - 1, y]
                and (x - 1, y) not in deads
                and (x - 1, y) not in rope
            ):
                rope.append(current)
                move(deads, (x - 1, y), value, rope, current)  # Moving.
        if (x, y - 1) in cosystem:  # Trying to move up.
            if (
                value == cosystem[x, y - 1]
                and (x, y - 1) not in deads
                and (x, y - 1) not in rope
            ):
                rope.append(current)
                move(deads, (x, y - 1), value, rope, current)  # Moving.
        if (x + 1, y) in cosystem:  # Trying to move right.
            if (
                value == cosystem[x + 1, y]
                and (x + 1, y) not in deads
                and (x + 1, y) not in rope
            ):
                rope.append(current)
                move(deads, (x + 1, y), value, rope, current)  # Moving.
        if (x, y + 1) in cosystem:  # Trying to move down.
            if (
                value == cosystem[x, y + 1]
                and (x, y + 1) not in deads
                and (x, y + 1) not in rope
            ):
                rope.append(current)
                move(deads, (x, y + 1), value, rope, current)  # Moving.
        if (
            rope
        ):  # This checks if we moved ever. If we did not move we don't add our current cell to neighbourhood list. This method excludes neighbourless cells to have themselves as their neighbour.
            deads.append(current)
            if before == current:
                return
            else:
                rope.remove(
                    before
                )  # Removing our previous cell so we can move back to it without problems. This part is a detail :3
        if (
            not rope and deads and before == current
        ):  # Finally adding our starting cell to neighbourhood if there is an neighbourhood which means we did move to other cells and add them to neighbourhood.
            deads.append(current)
            return

    for (
        particle
    ) in (
        cosystem
    ):  # For every particle in coordinate system we are going to run neighbourhood creator.
        rope = []  # Setting our current move branch to an empty list.
        deads = []  # Setting our neighbourhood list to an empty list.
        value = cosystem[
            particle
        ]  # Getting our starting cell value. This will be used to check every cell we are trying to move to have same value as our starting cell.
        move(deads, particle, value, rope, particle)  # Starting calculations.
        deads = set(
            deads
        )  # Converting neighbourhood to a set because no one can exist in their neighbourhood twice :)
        nesystem[
            particle
        ] = deads  # Attaching our neighbourhood to neighbourhood system dic with key of current cell.
    holder = nesystem.copy()
    for (
        elem
    ) in (
        nesystem
    ):  # Now we remove alone cells from neighbourhood system dic. Alone cells can't exist in a neighbourhood :(
        if not nesystem[elem]:
            holder.pop(elem)
    nesystem = holder.copy()


def popper(
    selection,
):  # This function is the main operator function of the game. It calculates scores and raises some errors for given input, however prints nothing because I currently don't know if we should print other errors.
    global gameon
    bottomPoint = 0
    gameon = True
    for elem in cosystem:  # Calculating the bottom point of coordinate system.
        if elem[1] > bottomPoint:
            bottomPoint = elem[1]
    mostRight = 0
    for elem in cosystem:  # Calculating the most right point of coordinate system.
        if elem[0] > mostRight:
            mostRight = elem[0]
    if (
        selection[0] <= mostRight and selection[1] <= bottomPoint
    ):  # Checking if given input is in the coordinate system domain. There is no else condition currently.
        gotval = True
        try:
            a = cosystem[
                selection
            ]  # Trying for this makes us realise if the cell is an empty cell or not. If it is an empty cell we print an error saying 'Please enter a correct size!'.
        except KeyError:
            gotval = False
        if gotval:
            try:
                error = True
                a = nesystem[
                    selection
                ]  # Trying for this makes us realise if the cell is alone or not since we already remove alone cells from neighbourhood dic. If it is alone we get KeyError and we set our parameter to False. Later on we check our parameter and continue. I do not know if there is a command for this and so we can do it with out using a parameter. If there is please notice me :) also as same with other errors there is no print message, currently.
            except KeyError:
                error = False
                current_State_Of_Economy = "You are a neighborless child!"
            finally:
                if error:
                    global score  # Calculating score before operating our game.

                    def fibonacci(
                        n,
                    ):  # Fibonacci function for using in score calculation.
                        if n == 0:
                            return 0
                        elif n == 1:
                            return 1
                        else:
                            return fibonacci(n - 1) + fibonacci(n - 2)

                    score = (
                        cosystem[selection] * fibonacci(len(nesystem[selection]))
                    ) + score  # Setting our score to 'score+(number in current cell)*fibonacci(total cell number in neighbourhood of current cell)'
                    for elem in nesystem[
                        selection
                    ]:  # Operating our game and popping all cells that included in the neighbour hood.
                        cosystem.pop(elem)
        else:
            print("Please enter a correct size!")
            gameon = False
            return
    else:
        print("Please enter a correct size!")
        gameon = False
        return


def rearrenge():  # This main function does rearrange our current coordinate system. Parameters are; moving down all cells as if there is a gravity. Also moving all columns to left if there is empty space. After we are done we set current coordinates of cells in coordinate system again by starting from top row being row number 1 and most left column being column number 1.
    global cosystem
    for times in range(
        ((maxy**2) + 1) // 2
    ):  # I am not sure but I try to make it as minimum as I thought it can be. This is the times we run our code for gravity function.
        holder = cosystem.copy()
        for elem in cosystem:  # Getting every existing cell.
            currentx, currenty = elem
            if (
                currentx,
                currenty + 1,
            ) not in cosystem and currenty < maxy:  # Checking if their below is empty.
                holder.pop(elem)
                holder[currentx, currenty + 1] = cosystem[
                    elem
                ]  # Moving them down if their below is empty.
        cosystem = holder.copy()
    for times in range(
        maxx + 2
    ):  # I am not sure but I try to make it as minimum as I thought it can be. This is the times we run our code for aligning coordinate system to row number 1.
        topPoint = maxy
        for elem in cosystem:  # Calculating our top point of function.
            if elem[1] < topPoint:
                topPoint = elem[1]
        if (
            topPoint != 1
        ):  # If top point is not row number 1 then we start to run our aligner function.
            holder = {}
            for elem in cosystem:
                holder[elem[0], elem[1] - 1] = cosystem[
                    elem
                ]  # Moving every cell 1 row upward. We will repeat this maximum row number + 2 times if top point is not row number 1 or until top point is becoming row number 1.
            cosystem = holder.copy()
    for times in range(
        ((maxy**2) + 1) // 2
    ):  # I am not sure but I try to make it as minimum as I thought it can be. This is the times we run our code for left sided column gravity function.
        moveLeft = False  # Setting a move left parameter.
        bottomPoint = 0
        for (
            elem
        ) in (
            cosystem
        ):  # Calculating our current bottom point row number. In other words our maximum row number of coordinate system.
            if elem[1] > bottomPoint:
                bottomPoint = elem[1]
        for x in range(2, maxx + 1):  # Getting numbers of columns from range function.
            for elem in cosystem:  # Getting every element of coordinate system.
                if (
                    elem[1] == bottomPoint
                ):  # Checking if they are the bottom point of their column.
                    if (
                        elem[0] == x
                    ):  # Checking if it is the current column we are examining.
                        try:
                            a = cosystem[
                                x - 1, elem[1]
                            ]  # Trying to realise if the column left ot it is empty or not.
                        except KeyError:
                            moveLeft = True  # Setting our parameter to True if the cell at left of our bottom cell of current examined column is empty.
            if moveLeft:  # If it is we are going to move this column to left.
                holder = cosystem.copy()
                for elem in cosystem:  # Getting every element of coordinate system.
                    if (
                        elem[0] == x
                    ):  # Checking if they are in the current column we are moving.
                        holder.pop(elem)
                        holder[elem[0] - 1, elem[1]] = cosystem[
                            elem
                        ]  # Moving our whole column.
                cosystem = holder.copy()


#  These functions above probably are way too complex than they should be but sometimes the the most simple answer is not the righteous answer.


neighbourhoodcreator()  # Running neighbourhood creator function for the first time before we start our game running.


while (
    gameover == False
):  # Starting to run our game in condition of game over being False.
    if gameon:
        for lineno in range(
            1, maxy + 1
        ):  # Reading our data of coordinate system dic and printing it in to given output format.
            for columno in range(1, maxx + 1):
                done = False
                for elem in cosystem:
                    if elem[1] == lineno and elem[0] == columno:
                        print(str(cosystem[elem]), end=" ")
                        done = True
                if done == False:
                    print(" ", end=" ")
            print(end="\n")
        print("")
        print("Your score is: " + str(score))  # Printing results for our score.
    nesystem = {}  # Emptying the neighbourhood system dic.
    neighbourhoodcreator()  # Running our neighbourhood creator.
    if (
        not nesystem
    ):  # Checking if neighbourhood system dic is empty or not. If it is empty there is no valid operations that can be made, in other words GAMEOVER! :D
        print("")
        print("Game over")
        gameover = True
        break
    print("")
    command = input(
        "Please enter a row and column number: "
    )  # Getting our input from user.
    print("")
    if (
        command.casefold() == "stop"
        or command.casefold() == "quit"
        or command.casefold() == "exit"
    ):  # These are easter eggs of our code. This one is for quiting game. Use it if you are tired of playing.
        quit()
    if (
        command.casefold() == "show"
        or command.casefold() == "moves"
        or command.casefold() == "helpme"
    ):  # Another easter egg command. It gives you hints for moves you can make.
        print("You can make one of the following moves: ", end="")
        for x in nesystem:
            a = str(x).replace(",", "").strip("(").strip(")")
            print(a, end=", ")
        print(end="\n")
        print("")
    if command.casefold() == "help":  # General help command.
        print(
            """       - To break out of game simply write stop or exit or quit. Letter case independent.
       - To see all available moves write show or moves or helpme. Letter case independent.
       - To see all available command write help. Letter case independent
       - Your input must be like 'x y' where x and y are integers and separated by one space. Otherwise you will be asked for input again and again.
       - You will not receive error message except if your selection is in game's range but empty.
"""
        )
    command = command.split(" ")
    try:
        command = tuple(
            int(x) for x in command
        )  # Converting input to an integer tuple.
        x, y = command
        command = y, x
    except TypeError:  # Detecting possible errors. But no error message given.
        continue
    except ValueError:
        continue
    if (
        len(command) == 2
        and isinstance(command[1], int)
        and isinstance(command[0], int)
    ):  # Detecting other possible errors. Still no error message given for them.
        popper(command)  # Executing current input with operator function.
        rearrenge()  # Rearranging our coordinate system.
input_file.close()
# End of my code. This assignment was fun to code. Thank you for great ideas.
