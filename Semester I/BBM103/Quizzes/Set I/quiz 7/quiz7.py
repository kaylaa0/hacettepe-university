# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

try:
    input = open(sys.argv[1], "r")
except IOError:
    # IOError message:
    print("IOError: cannot open", str(sys.argv[1]))
    print("\n~ Game Over ~")
    sys.exit()
except FileNotFoundError:
    print("IOError: cannot open", str(sys.argv[1]))
    print("\n~ Game Over ~")
    sys.exit()
except IndexError:
    # IndexError message for missing file names:
    print("IndexError: number of input files less than expected.")
    print("\n~ Game Over ~")
    sys.exit()
except:
    print("kaBOOM: run for your life!")
try:
    match = open(sys.argv[2], "r")
except IOError:
    # IOError message:
    print("IOError: cannot open", str(sys.argv[2]))
    print("\n~ Game Over ~")
    sys.exit()
except FileNotFoundError:
    print("IOError: cannot open", str(sys.argv[2]))
    print("\n~ Game Over ~")
    sys.exit()
except IndexError:
    # IndexError message for missing file names:
    print("IndexError: number of input files less than expected.")
    print("\n~ Game Over ~")
    sys.exit()
except:
    print("kaBOOM: run for your life!")
try:
    for line, line2 in zip(input, match):
        print("------------")
        my_results = []
        try:
            line = line.split(" ")
        except TypeError:
            print("a")
        except:
            print("kaBOOM: run for your life!")
        a = line[3:4]
        detered = " ".join(line)
        detered = detered.strip("\n")
        try:
            assert len(line) == 4
        except AssertionError:
            print("IndexError: number of operands less than expected.")
            print("Given input:", detered)
            continue
        except:
            print("kaBOOM: run for your life!")
        try:
            determ1 = 0 + float("".join(line[:1]))
        except ValueError:
            # ValueError  message:
            print("ValueError: only numeric input is accepted.")
            print("Given input:", detered)
            continue
        except:
            print("kaBOOM: run for your life!")
        try:
            determ1 = 0 + float("".join(line[1:2]))
        except ValueError:
            # ValueError  message:
            print("ValueError: only numeric input is accepted.")
            print("Given input:", detered)
            continue
        except:
            print("kaBOOM: run for your life!")
        try:
            determ1 = 0 + float("".join(line[2:3]))
        except ValueError:
            # ValueError  message:
            print("ValueError: only numeric input is accepted.")
            print("Given input:", detered)
            continue
        except:
            print("kaBOOM: run for your life!")
        try:
            determ1 = 0 + float("".join(line[3:4]))
        except ValueError:
            # ValueError  message:
            print("ValueError: only numeric input is accepted.")
            print("Given input:", detered)
            continue
        except:
            print("kaBOOM: run for your life!")
        div = round(float("".join(line[:1])) - 0.49)
        notdiv = round(float("".join(line[1:2])) - 0.49)
        trick = 1
        for i in range(
            round(float("".join(line[2:3])) - 0.49),
            round(float("".join(line[3:4])) + 0.51),
        ):
            try:
                if i % div == 0 and i % notdiv != 0:
                    my_results.insert(len(my_results), i)
            except ZeroDivisionError:
                print("ZeroDivisionError: You can't divide by 0.")
                print("Given input:", detered)
                trick = 0
                break
            except (i % div == 0) is False:
                continue
            except:
                print("kaBOOM: run for your life!")
        my_results = " ".join(str(i) for i in my_results)
        my_results = str(my_results)
        trick == 0 or print("My results:\t\t", my_results)
        trick == 0 or print("Results to compare:\t", ("".join(line2)).strip("\n"))
        trap = 1
        try:
            assert my_results == ("".join(line2)).strip("\n")
        except AssertionError:
            print("Assertion Error: results don't match.")
            trap = 0
        except:
            print("kaBOOM: run for your life!")
        trap == 0 or print("Goool!!!")
except:
    print("kaBOOM: run for your life!")
try:
    input.close()
    match.close()
except:
    print("kaBOOM: run for your life!")
print("\n~ Game Over ~")
