# Hacettepe University
# Kayla Akyüz
# 21726914

import sys  # importing necessary modules
import string

open("out.txt", "w").close()  # clearing output file if it exist already and filled
sys.stdout = open("out.txt", "r+")  # changing print direction to output file

alphabet = dict(
    enumerate(string.ascii_uppercase, 1)
)  # getting a numeric list of alphabet
halls = {}


def CREATEHALL(hallname, number):  # creating CREATEHALL command
    if hallname in halls:
        print(
            "Warning: Cannot create the hall for the second time. The cinema has already "
            + hallname
        )  # Checking if hall exist and giving error if it does.
    else:
        halls[
            hallname
        ] = number  # putting hallname and corresponding number in main dict.
        c = number.split("x")  # calculating total seats
        a = int(c[0])
        if a > 26:
            print(
                "You have given a number bigger than 26 for rows. Rows are named after English alphabet as a result, number of the rows of any hall is limited up to 26. Please chose a number below 27."
            )  # Checking if given row number is bigger than 26. If it is raising and error and exiting function.
            return False
        b = int(c[1])
        globals()[
            hallname
        ] = (
            {}
        )  # creating a global dict of given hallname. This dict will have X X X X etc. as variable so it can be used as visualized hall.
        for y in range(1, a + 1):
            current = []
            for x in range(1, b + 1):
                current.append("X")
            globals()[hallname][
                alphabet[y]
            ] = current  # converting numbers to letters and creating our hall
        print(
            "Hall '" + hallname + "' having " + str(a * b) + " seats has been created"
        )  # printing out the process is successful


def SELLTICKET(name, type, hall, seat):  # creating SELLTICKET command
    if hall not in halls.keys():
        print(
            "The hall '" + hall + "' is not an existing hall."
        )  # checking if hall exist and giving error if it does not.
        return False  # exiting function if error is given.
    elif "-" not in seat:  # Checking if we are doing multiple or solo operation
        if seat[0] not in globals()[hall].keys():
            print(
                "Error: The hall '"
                + hall
                + "' has less row than the specified index "
                + seat
                + "!"
            )  # checking if given row number is valid
            return False
        elif int(seat[1:]) > len(globals()[hall][seat[0]]):
            print(
                "Error: The hall '"
                + hall
                + "' has less column than the specified index "
                + seat
                + "!"
            )  # checking if given column number is valid
            return False
        elif globals()[hall][seat[0]][int(seat[1:])] != "X":
            print(
                "Warning: The seat "
                + seat
                + " cannot be sold to "
                + name
                + " since it was already sold!"
            )  # checking if the seat is sold if it is giving error.
        else:
            globals()[hall][seat[0]][int(seat[1:])] = type[0].upper()
            print(
                "Success: " + name + " has bought " + seat + " at " + hall
            )  # printing result of successful operation.
    elif "-" in seat:
        c = seat[1:].split("-")
        if seat[0] not in globals()[hall].keys():
            print(
                "Error: The hall '"
                + hall
                + "' has less row than the specified index "
                + seat
                + "!"
            )
            return False
        elif int(c[1]) > len(globals()[hall][seat[0]]):
            print(
                "Error: The hall '"
                + hall
                + "' has less column than the specified index "
                + seat
                + "!"
            )
            return False
        door = True
        for times in range(int(c[0]), int(c[1])):
            if (
                globals()[hall][seat[0]][int(times)] != "X"
            ):  # checking if there is an already sold seat in given range if there is making do/or variable false.
                door = False
            else:
                continue
        if door == False:
            print(
                "Warning: The seats "
                + seat
                + " cannot be sold to "
                + name
                + " due some of them have already been sold!"
            )  # checking do/or variable and if it is false making a warning because it means there are some seats that has been already sold in given range.
        else:
            for times in range(int(c[0]), int(c[1])):
                globals()[hall][seat[0]][int(times)] = type[0].upper()
            print(
                "Success: " + name + " has bought " + str(seat) + " at " + hall
            )  # printing results


def CANCELTICKET(hall, seat):  # creating CANCELTICKET command
    if hall not in halls.keys():
        print(
            "The hall '" + hall + "' is not an existing hall."
        )  # same checks with above
        return False
    elif "-" not in seat:  # checking if multiple or solo operation
        if seat[0] not in globals()[hall].keys():
            print(
                "Error: The hall '"
                + hall
                + "' has less row than the specified index "
                + seat
                + "!"
            )
            return False
        elif int(seat[1:]) > len(globals()[hall][seat[0]]):
            print(
                "Error: The hall '"
                + hall
                + "' has less column than the specified index "
                + seat
                + "!"
            )
            return False
        elif globals()[hall][seat[0]][int(seat[1:])] == "X":
            print(
                "Error: The seat "
                + seat
                + " at "
                + hall
                + " has already been free! Nothing to cancel"
            )  # checking if seat is already free or not. If it is giving error.
        else:
            globals()[hall][seat[0]][int(seat[1:])] = "X"
            print(
                "Success: The seat "
                + seat
                + " at "
                + hall
                + " has been canceled and now ready to be sold again"
            )  # printing successful operation
    elif "-" in seat:
        c = seat[1:].split("-")
        if seat[0] not in globals()[hall].keys():
            print(
                "Error: The hall '"
                + hall
                + "' has less row than the specified index "
                + str(seat)
                + "!"
            )
            return False
        elif int(c[1]) > len(globals()[hall][seat[0]]):
            print(
                "Error: The hall '"
                + hall
                + "' has less column than the specified index "
                + str(seat)
                + "!"
            )
            return False
        door = True
        handler = []
        for times in range(int(c[0]), int(c[1])):
            if (
                globals()[hall][seat[0]][int(times)] == "X"
            ):  # checking if there is already canceled seat in given range. If there is output will be separately for each seat given warning or success. Otherwise all canceled with 1 output.
                door = False
            else:
                continue
        if door == False:
            for times in range(int(c[0]), int(c[1])):
                if globals()[hall][seat[0]][int(times)] == "X":
                    print(
                        "Error: "
                        + "The seat "
                        + str(seat[0])
                        + str(times)
                        + " at "
                        + str(hall)
                        + "  has already been free! Nothing to cancel"
                    )
                else:
                    print(
                        "Success: "
                        + "The seat "
                        + str(seat[0])
                        + str(times)
                        + " at "
                        + str(hall)
                        + " has been canceled and now ready to sell again"
                    )
            for times in range(int(c[0]), int(c[1])):
                globals()[hall][seat[0]][int(times)] = "X"
        else:
            for times in range(int(c[0]), int(c[1])):
                globals()[hall][seat[0]][int(times)] = "X"
            print(
                "Success: "
                + "The seat "
                + str(seat)
                + " at "
                + str(hall)
                + " has been canceled and now ready to sell again"
            )


def BALANCE(hall):  # creating BALANCE command
    balances = 0  # total balance of student ticket sold variable
    balancef = 0  # total balance of full ticket sold variable
    for x in globals()[
        hall
    ].values():  # viewing hall dict and increasing student and full balance variables if there is 'S' for student (5) or 'F' for full (10)
        for y in x:
            if y == "S":
                balances = balances + 5
            elif y == "F":
                balancef = balancef + 10
            else:
                continue  # May be something else. Other type of ticket etc. They are not calculated. I think if there was a CREATETICKETTYPE function the work would be more beautiful.
    print(
        """Hall report of '"""
        + hall
        + """'
-------------------------
Sum of students = """
        + str(balances)
        + """, Sum of full fares = """
        + str(balancef)
        + """, Overall = """
        + str(balances + balancef)
    )  # Calculating total and printing all results.


def SHOWHALL(hall):  # creating SHOWHALL command
    print("Printing hall layout of " + hall)  # printing that the function is commencing
    for row in sorted(
        globals()[hall].keys(), reverse=True
    ):  # reverse sorting the hall dict. While we are printing it will be useful because print goes reverse from top to bottom.
        print(
            row, "  ".join(globals()[hall][row])
        )  # printing hall dict. key is row letter value is 'X,S,F's joined by 2 space
    columnlen = [
        x for x in globals()[hall].values()
    ]  # now printing last line which is numbers of columns. I don't understand why it starts from 0 but it does. The real trick here is that until 10 there 2 space between numbers but after 10 numbers themselves become 2 digit and there has to be 1 space between them so it look oriented. I also coded so that after 100 there wont be space between numbers and it will look oriented too but after 1000 columns there is no solution. Is there a cinema more than 1000 columns anyway ? :3
    columnlen = len(columnlen[1])
    if columnlen < 10:
        for x in range(0, columnlen):
            print(" ", x, end="")
    elif 10 <= columnlen < 100:
        for xc in range(0, 10):
            print(" ", xc, end="")
        for xd in range(10, columnlen):
            print("", xd, end="")
    else:
        list = []
        for xp in range(0, 10):
            print(" ", xp, end="")
        for xl in range(10, 100):
            print("", xl, end="")
        for xx in range(100, columnlen):
            list.append(str(xx))
        print("".join(list))
    print(end="\n")


input = open(sys.argv[1], "r")  # reading input argv for input text file and opening it.
for line in input:  # for line in input file commencing them
    line = line.strip("\n")  # have to clear that \n at the end.
    line = line.split(" ")  # and split them so we can commands read separately
    if (
        line[0] == "CREATEHALL"
    ):  # checking if command is CREATEHALL and if it is commencing defined function
        CREATEHALL(line[1], line[2])
    elif (
        line[0] == "SELLTICKET"
    ):  # checking if command is SELLTICKET and if it is commencing defined function
        try:
            if len(line) > 5:
                for x in range(
                    4, len(line)
                ):  # checking if there is more than 1 operation of tickets sold to person. If there is we will have all operations in a loop.
                    SELLTICKET(line[1], line[2], line[3], line[x])
            else:
                SELLTICKET(
                    line[1], line[2], line[3], line[4]
                )  # if there is 1 operation quickly commencing it. I don't know why I didn't included these 2 in 1 range loop. While I was coding this was easier and I am too lazy to change it afterwards.
        except KeyError:
            print(
                "Error: There is a KeyError. Possible cause is: The hall '"
                + line[3]
                + "' is not an existing hall."
            )  # There is a KeyError probably because of hall name not existing.
    elif (
        line[0] == "CANCELTICKET"
    ):  # checking if command is CANCELTICKET and if it is commencing defined function
        try:
            if len(line) > 3:
                for x in range(
                    2, len(line)
                ):  # same with SELLTICKET command if there is multiple operations having them in a loop.
                    CANCELTICKET(line[1], line[x])
            else:
                CANCELTICKET(line[1], line[2])
        except KeyError:
            print(
                "Error: There is a KeyError. Possible cause is: The hall '"
                + line[1]
                + "' is not an existing hall."
            )
    elif (
        line[0] == "BALANCE"
    ):  # checking if command is BALANCE and if it is commencing defined function.
        if len(line) > 2:
            for x in range(
                1, len(line)
            ):  # If there is more than 1 balance to show having them in a loop.
                try:
                    BALANCE(line[x])
                except KeyError:
                    print(
                        "Error: There is a KeyError. Possible cause is: The hall '"
                        + line[x]
                        + "' is not an existing hall."
                    )  # checking error in the loop so giving error for only hall that gives the error.
        else:
            try:
                BALANCE(line[1])
            except KeyError:
                print(
                    "Error: There is a KeyError. Possible cause is: The hall '"
                    + line[1]
                    + "' is not an existing hall."
                )
    elif (
        line[0] == "SHOWHALL"
    ):  # checking if command is SHOWHALL and if it is commencing defined function.
        if len(line) > 2:
            for x in range(
                1, len(line)
            ):  # Checking if there is more than 1 hall to show if there is having them in a loop.
                try:
                    SHOWHALL(line[x])
                except KeyError:
                    print(
                        "There is a KeyError. Possible cause is: The hall '"
                        + line[x]
                        + "' is not an existing hall."
                    )
        else:
            try:
                SHOWHALL(line[1])
            except KeyError:
                print(
                    "There is a KeyError. Possible cause is: The hall '"
                    + line[1]
                    + "' is not an existing hall."
                )

    elif line[0] == "\n":  # These parts I add last to my homework
        continue
    elif (
        line[0] == ""
    ):  # They are for checking input file for other commands that not existing.
        continue
    else:  # and it prints this if there is some random line or different command.
        print(
            """Given command is not a valid command. Try exact inputs from given formats at below:
            CREATEHALL A BxC : Creating a hall called A with BxC domain. B is row number and cannot be bigger than 26. C is line number and if it is bigger than 999 output will be corrupted.
            SELLTICKET A B C D : Perform ticket sell for person called A in hall called C for range D and ticket type B.
            CANCELTICKET A B : Perform ticket cancel for seats in range B and in hall A.
            BALANCE A : Show current balance of given hall A.
            SHOWHALL A : Showing a visual representation of given hall A. 
            
            Note: You can perform multiple operations for SELLTICKET by giving more than one D input.
                  You can perform multiple operations for CANCELTICKET by giving more than one B input.
                  You can perform multiple operations for BALANCE by giving more than one A input.
                  You can perform multiple operations for SHOWHALL by giving more than one A input.
              """
        )
# Now tricky part starts. Every print command we have commenced above has been printed into out.txt. But assignment requires to have print results also in console. When I started my coding I was not aware of this situation. If I was I would probably make 2 print statements every time and one of them to console other one to out.txt. But it was too much work to do so I code this method.
sys.stdout = (
    sys.__stdout__
)  # chaning print directory back to console. After this command every print command will be printed to console.
output = open("out.txt", "r")  # opening out.txt as readable file
try:
    a = output.read()[
        -1
    ]  # now trying if there is a last line in out.txt. If there is not probably no input given or some other terrible stuff happened with the code.
except IndexError:
    print(
        "Output file is empty or corrupted. Nothing to print."
    )  # Giving and error if there is not a last line because that means out.txt is empty. Hopefully it does not happen.
    sys.exit()  # quiting the python file. Because if out.txt is empty we can't commence upcoming commands.
output = open(
    "out.txt", "r"
)  # opening out.txt again. Because it is closed while we are trying to see if it is empty or not.
if (
    a == "\n"
):  # now if last line is \n we are gonna discard it so print statement looks better. At first I was trying to mach output format but later on I realise output is not important. However I am going to keep this because it looks better this way.
    print(output.read()[:-1])  # printing out.txt with discarding last empty line.
else:
    print(output.read())  # if last line is not \n printing out.txt directly.
output.close()  # closing out.txt file
# End of my code. Hopefully it was clear and understandable. Thank you for reading comments. Really great homework it was fun to code. <3
