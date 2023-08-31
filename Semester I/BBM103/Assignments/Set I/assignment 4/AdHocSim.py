# Hacettepe University
# Kayla Akyüz
# 21726914

import sys
import math

# from collections import OrderedDict

print("********************************")
print("AD-HOC NETWORK SIMULATOR - BEGIN")
print("********************************")
print(
    "SIMULATION TIME: "
    + str(0).zfill(2)
    + ":"
    + str(0).zfill(2)
    + ":"
    + str(0).zfill(2)
)
commands = open("commands", "r")
packetsize = sys.argv[1]
node_listn = []
node_list = {}
current_branch = []
branch_list = []
cbranch_list = []
current_cbranch = []
node_dict = {}
cost_call = {}
cost_list = {}
route_printc = {}
the_amount = []
route_print = {}
cost_callc = {}
node_listr = []
node_dicts = {}
route_printr = {}
packet_counter = 1
route_printcr = {}
node_sort = []
node_lists = {}
# route_number=1
time_vortex = {}


def CLEANREST():
    global route_printcr
    global route_print
    global route_printr
    global route_printc
    global cost_call
    global cost_callc
    global node_dict
    global cost_list
    global branch_list
    global node_listr
    global current_branch
    global cbranch_list
    global the_amount
    global current_cbranch
    global cost_list
    cost_list = {}
    current_branch = []
    node_listr = []
    route_printr = {}
    cost_callc = {}
    node_dict = {}
    branch_list = []
    cbranch_list = []
    current_cbranch = []
    route_print = {}
    route_printc = {}
    route_printcr = {}
    cost_call = {}
    the_amount = []
    cost_list = {}
    for key, value in node_list.items():
        node_dict.update({key: []})


def SORT():
    global node_dicts
    global node_sort
    global node_listr
    global node_listn
    node_listr = sorted(node_listr)
    node_listn = sorted(node_listn)
    node_sort = sorted(node_dicts.keys(), key=lambda x: x.lower())


def CRNODE(name, posx, posy, rane, ranw, rann, rans, charge):
    node_listn.append(name)
    node_list.update(
        {
            name: {
                "posx": int(posx),
                "posy": int(posy),
                "rane": int(rane) + int(posx),
                "ranw": int(posx) - int(ranw),
                "rann": int(rann) + int(posy),
                "rans": int(posy) - int(rans),
                "charge": int(charge),
                "prane": int(rane),
                "pranw": int(ranw),
                "prann": int(rann),
                "prans": int(rans),
            }
        }
    )
    node_dict.update({name: []})


def MOVE(name, posx, posy):
    node_list[name]["posx"] = int(posx)
    node_list[name]["posy"] = int(posy)
    node_list[name]["rane"] = node_list[name]["prane"] + int(posx)
    node_list[name]["ranw"] = int(posx) - int(node_list[name]["pranw"])
    node_list[name]["rann"] = node_list[name]["prann"] + int(posy)
    node_list[name]["rans"] = int(posy) - int(node_list[name]["prans"])
    # print(node_list[name]['rans'],node_list[name]['rane'])


def CHBTTRY(name, val):
    node_list[name]["charge"] = int(val)


def RMNODE(name):
    del node_list[name]
    node_listn.remove(name)
    del node_dict[name]


def NDEF(node_listr, itemm):
    def NDEFT(itm, nghbr):
        if len(node_listr) == 0:
            return
        else:
            node_listr.remove(nghbr)
            if int(node_list[itm]["ranw"]) <= int(node_list[nghbr]["posx"]) <= int(
                node_list[itm]["rane"]
            ) and int(node_list[itm]["rans"]) <= int(node_list[nghbr]["posy"]) <= int(
                node_list[itm]["rann"]
            ):
                neighood = node_dict[itm]
                neighood.insert(len(neighood), nghbr)
                node_dict.update({itm: neighood})
                # print(node_listr)
                for nghbr in sorted(node_listr):
                    # print(nghbr)
                    NDEFT(itm, nghbr)
            else:
                for nghbr in node_listr:
                    NDEFT(itm, nghbr)

    node_listr.remove(itemm)
    # print(node_listr)
    # node_listr=sorted(node_listr)
    # print(node_listr)
    for nghbr in sorted(node_listr):
        NDEFT(itemm, nghbr)


def STEPCALC(frm, to):
    x1 = node_list[frm]["posx"]
    y1 = node_list[frm]["posy"]
    x2 = node_list[to]["posx"]
    y2 = node_list[to]["posy"]
    bati = node_list[to]["charge"]
    distance = math.hypot(x2 - x1, y2 - y1)
    cost = distance / bati
    return cost


def CALC(frm, to, cfrm, cto):
    global route_printcr
    global route_print
    global route_printr
    global route_printc
    global cost_call
    global node_dict
    global cost_list
    global branch_list
    global node_listr
    global current_branch
    global cbranch_list
    global the_amount
    global current_cbranch
    global cost_list
    # print(cbranch_list)
    # print(frm,to,cfrm,cto)
    global route_number
    cost = STEPCALC(cfrm, cto)
    if frm == cfrm and to == cto:
        for itm in node_list:
            for i in node_list:
                node_listr.insert(len(node_listr), i)
            NDEF(node_listr, itm)
        route_number = 1
        if to in node_dict[frm]:
            branch_list.insert(len(branch_list), frm)
            branch_list.insert(len(branch_list), to)
            return
        current_branch.insert(len(current_branch), frm)
        for i in node_dict[frm]:
            CALC(frm, to, frm, i)
    elif to in node_dict[cto]:
        for element in node_dict[cto]:
            if element == to:
                current_branch.insert(len(current_branch), cto)
                current_branch.insert(len(current_branch), to)
                current_cbranch.insert(len(current_cbranch), cost)
                fcost = STEPCALC(cto, to)
                current_cbranch.insert(len(current_cbranch), fcost)
                branch_list.insert(len(branch_list), current_branch)
                cbranch_list.insert(len(cbranch_list), current_cbranch)
                cost_call.update({str(current_branch): str(current_cbranch)})
                route_print.update({route_number: "->".join(current_branch)})
                route_printc.update({"->".join(current_branch): str(current_branch)})
                route_number = route_number + 1
                current_branch.remove(current_branch[-1])
                current_branch.remove(current_branch[-1])
                current_cbranch.remove(current_cbranch[-1])
                current_cbranch.remove(current_cbranch[-1])
            else:
                if element in current_branch:
                    continue
                elif element == cto:
                    continue
                else:
                    """
                    print(current_branch)
                    print(current_cbranch)
                    current_branch.insert(len(current_branch), cto)
                    current_branch.insert(len(current_branch), element)
                    current_cbranch.insert(len(current_cbranch), cost)
                    cost = STEPCALC(cto, element)
                    current_cbranch.insert(len(current_cbranch), cost)
                    CALC(frm, to, cto, element)
                    current_branch.remove(current_branch[-1])
                    current_branch.remove(current_branch[-1])
                    current_cbranch.remove(current_cbranch[-1])
                    current_cbranch.remove(current_cbranch[-1])
                    """
        return
    elif len(node_dict[cto]) == 0:
        # print(node_dict[cto])
        # print(node_dict)
        # print(node_list)
        # print(len(node_dict[cto]))
        if cto == to:
            current_branch.insert(len(current_branch), cto)
            current_cbranch.insert(len(current_cbranch), cost)
            branch_list.insert(len(branch_list), current_branch)
            cbranch_list.insert(len(cbranch_list), current_cbranch)
            cost_call.update({str(current_branch): str(current_cbranch)})
            route_print.update({route_number: "->".join(current_branch)})
            route_printc.update({"->".join(current_branch): str(current_branch)})
            route_number = route_number + 1
            current_branch.remove(current_branch[-1])
            current_cbranch.remove(current_cbranch[-1])
            return
        elif cto != to:
            if cto in current_branch:
                return
            else:
                current_branch.insert(len(current_branch), cto)
                current_cbranch.insert(len(current_cbranch), cost)
                for i in node_dict[cto]:
                    CALC(frm, to, cto, i)
                current_branch.remove(current_branch[-1])
                current_cbranch.remove(current_cbranch[-1])
            return
    elif cto != to:
        if cto in current_branch:
            return
        else:
            current_branch.insert(len(current_branch), cto)
            current_cbranch.insert(len(current_cbranch), cost)
            for i in node_dict[cto]:
                CALC(frm, to, cto, i)
            current_branch.remove(current_branch[-1])
            current_cbranch.remove(current_cbranch[-1])
        return


def CLEARCOST():
    global cost_callc
    for key, value in cost_call.items():
        # print(value)
        if "," in value:
            value = value[1:-1].strip(",").split(" ")
        else:
            value = value[1:-1].split(" ")
        nval = 0
        for i in value:
            i = i.strip(",")
            i = float(i)
            nval = nval + i
        cost_callc.update({key: nval})


def SWITCHDIC(dict, outdic):
    for key, value in dict.items():
        # print(value)
        if value in outdic:
            holder = [outdic[value], key]
            outdic.update({value: min(holder, key=len)})
        else:
            outdic.update({value: key})


def GETMIN(dict):
    global frm
    global to
    try:
        the_min = []
        for key, value in dict.items():
            a = value
            the_min.insert(0, a)
            the_amount.insert(0, a)
        return min(the_min)
    except ValueError:
        print("\tNODES & THEIR NEIGHBORS:", end=" ")
        for key in node_listn:
            value = node_dict[key]
            print(str(key) + " -> ", end="")
            print(", ".join(value) + " | ", end="")
        print(end="\n")
        print("\tNO ROUTE FROM " + str(routefrom) + " TO " + str(routeto) + " FOUND.")
        sys.exit()


def NODELISTING():
    print("\tNODES & THEIR NEIGHBORS:", end=" ")
    for key in node_listn:
        value = node_dict[key]
        print(str(key) + " -> ", end="")
        print(", ".join(value) + " | ", end="")
    print(end="\n")
    print("\t" + str(len(the_amount)) + " ROUTE(S) FOUND:")
    for number, route in route_print.items():
        routecall = route_printc[route]
        cost = cost_callc[routecall]
        cost = float(cost)
        print(
            "\tROUTE "
            + str(number)
            + ": "
            + str(route)
            + "\t COST: {0:.4f}".format(cost)
        )
    SWITCHDIC(route_printc, route_printcr)
    route = GETMIN(cost_callc)
    SWITCHDIC(route_print, route_printr)
    holder = cost_list[route]
    hold3r = route_printcr[holder]
    routep = route_printr[hold3r]
    thefnsh = route_printcr[holder]
    # print(cost_list)
    print("\tSELECTED ROUTE (ROUTE " + str(routep) + "): " + str(thefnsh))


def SEND(frm, to, ds):
    global routefrom
    global routeto
    routeto = to
    routefrom = frm
    global packet_counter
    CALC(frm, to, frm, to)
    CLEARCOST()
    route = GETMIN(cost_callc)
    SWITCHDIC(cost_callc, cost_list)
    NP = int(ds) / int(packetsize)
    duration = round(NP - 0.9)
    for i in range(1, duration + 1):
        time_vortex.update({i: []})
    for line in commands:
        line = line.strip("\n")
        line = line.split("\t")
        if line[0] == "0":
            continue
        else:
            if time_vortex[int(line[0])] == []:
                time_vortex.update({int(line[0]): line[1:]})
            elif time_vortex[int(line[0])] != []:
                hold = [line[1:], time_vortex[int(line[0])]]
                time_vortex.update({int(line[0]): hold})
            """START THE TIME VORTEX"""
    NODELISTING()
    print("\tPACKET " + "1" + " HAS BEEN SENT")
    remainsiz = float(ds) - float(packetsize)
    print("\tREMAINING DATA SIZE: " + "{0:.6}".format(remainsiz) + " BYTE")
    for moment, dimention in time_vortex.items():
        remainsiz = float(remainsiz) - float(packetsize)
        packet_counter = packet_counter + 1
        moment = int(moment)
        min, sec = divmod(moment, 60)
        hour, min = divmod(min, 60)
        print(
            "SIMULATION TIME: "
            + str(hour).zfill(2)
            + ":"
            + str(min).zfill(2)
            + ":"
            + str(sec).zfill(2)
        )
        if dimention != []:
            if len(dimention) >= 1:
                if dimention[0] == "MOVE":
                    doctor = dimention[2].split(";")
                    MOVE(dimention[1], doctor[0], doctor[1])
                    print(
                        "\tCOMMAND *MOVE*: The location of node "
                        + str(dimention[1])
                        + " is changed"
                    )
                    CLEANREST()
                    CALC(frm, to, frm, to)
                    CLEARCOST()
                    GETMIN(cost_callc)
                    SWITCHDIC(cost_callc, cost_list)
                    NODELISTING()
                elif dimention[0] == "CHBTTRY":
                    CHBTTRY(dimention[1], dimention[2])
                    print(
                        "\tCOMMAND *CHBTTRY*: Battery level of node "
                        + str(dimention[1])
                        + " is changed to "
                        + str(dimention[2])
                    )
                    CLEANREST()
                    CALC(frm, to, frm, to)
                    CLEARCOST()
                    GETMIN(cost_callc)
                    SWITCHDIC(cost_callc, cost_list)
                    NODELISTING()
                elif dimention[0] == "RMNODE":
                    RMNODE(dimention[1])
                    print(
                        "\tCOMMAND *RMNODE*: Node " + str(dimention[1]) + " is removed"
                    )
                    CLEANREST()
                    CALC(frm, to, frm, to)
                    CLEARCOST()
                    GETMIN(cost_callc)
                    SWITCHDIC(cost_callc, cost_list)
                    NODELISTING()
                elif dimention[0] == "CRNODE":
                    post = dimention[2].split(";")
                    ran = dimention[3].split(";")
                    CRNODE(
                        dimention[1],
                        post[0],
                        post[1],
                        ran[0],
                        ran[1],
                        ran[2],
                        ran[3],
                        dimention[4],
                    )
                    print(
                        "\tCOMMAND *CRNODE*: New node "
                        + str(dimention[1])
                        + " is created"
                    )
                    CLEANREST()
                    CALC(frm, to, frm, to)
                    CLEARCOST()
                    GETMIN(cost_callc)
                    SWITCHDIC(cost_callc, cost_list)
                    NODELISTING()
                elif dimention[0] == "SEND":
                    print(
                        "\tCOMMAND *SEND*: Data is ready to send from "
                        + str(dimention[1])
                        + " to "
                        + str(dimention[2])
                    )
                    CLEANREST()
                    CALC(frm, to, frm, to)
                    CLEARCOST()
                    GETMIN(cost_callc)
                    SWITCHDIC(cost_callc, cost_list)
                    SEND(dimention[1], dimention[2], dimention[3])
                elif len(dimention) > 1:
                    for object in dimention:
                        if object[0] == "MOVE":
                            doctor = object[2].split(";")
                            MOVE(object[1], doctor[0], doctor[1])
                            print(
                                "\tCOMMAND *MOVE*: The location of node "
                                + str(object[1])
                                + " is changed"
                            )
                            CLEANREST()
                            CALC(frm, to, frm, to)
                            CLEARCOST()
                            GETMIN(cost_callc)
                            SWITCHDIC(cost_callc, cost_list)
                            NODELISTING()
                        elif object[0] == "CHBTTRY":
                            CHBTTRY(object[1], object[2])
                            print(
                                "\tCOMMAND *CHBTTRY*: Battery level of node "
                                + str(object[1])
                                + " is changed to "
                                + str(object[2])
                            )
                            CLEANREST()
                            CALC(frm, to, frm, to)
                            CLEARCOST()
                            GETMIN(cost_callc)
                            SWITCHDIC(cost_callc, cost_list)
                            NODELISTING()
                        elif object[0] == "RMNODE":
                            RMNODE(object[1])
                            print(
                                "\tCOMMAND *RMNODE*: Node "
                                + str(object[1])
                                + " is removed"
                            )
                            CLEANREST()
                            CALC(frm, to, frm, to)
                            CLEARCOST()
                            GETMIN(cost_callc)
                            SWITCHDIC(cost_callc, cost_list)
                            NODELISTING()
                        elif object[0] == "CRNODE":
                            post = object[2].split(";")
                            ran = object[3].split(";")
                            CRNODE(
                                object[1],
                                post[0],
                                post[1],
                                ran[0],
                                ran[1],
                                ran[2],
                                ran[3],
                                object[4],
                            )
                            print(
                                "\tCOMMAND *CRNODE*: New node "
                                + str(object[1])
                                + " is created"
                            )
                            CLEANREST()
                            CALC(frm, to, frm, to)
                            CLEARCOST()
                            GETMIN(cost_callc)
                            SWITCHDIC(cost_callc, cost_list)
                            NODELISTING()
                        elif object[0] == "SEND":
                            print(
                                "\tCOMMAND *SEND*: Data is ready to send from "
                                + str(object[1])
                                + " to "
                                + str(object[2])
                            )
                            SEND(object[1], object[2], object[3])
        print("\tPACKET " + str(packet_counter) + " HAS BEEN SENT")
        if remainsiz >= 0:
            print("\tREMAINING DATA SIZE: " + "{0:.6}".format(remainsiz) + " BYTE")
        elif remainsiz < 0:
            print("\tREMAINING DATA SIZE: " + "{0:.6}".format(0.0) + " BYTE")

    # print(duration)
    # cost_list[route]


for line in commands:
    line = line.split("\t")
    if line[0] == "0":
        if line[1] == "CRNODE":
            post = line[3].split(";")
            ran = line[4].split(";")
            CRNODE(
                line[2],
                post[0],
                post[1],
                ran[0],
                ran[1],
                ran[2],
                ran[3],
                line[5].strip("\n"),
            )
            print("\tCOMMAND *CRNODE*: New node " + str(line[2]) + " is created")
        elif line[1] == "MOVE":
            doctor = line[3].split(";")
            MOVE(line[2], doctor[0], doctor[1])
            print(
                "\tCOMMAND *MOVE*: The location of node " + str(line[2]) + " is changed"
            )
            NODELISTING()
        elif line[1] == "CHBTTRY":
            CHBTTRY(line[2], line[3])
            print(
                "\tCOMMAND *CHBTTRY*: Battery level of node "
                + str(line[2])
                + " is changed to "
                + str(line[3])
            )
            NODELISTING()
        elif line[1] == "RMNODE":
            RMNODE(line[2])
            print("\tCOMMAND *RMNODE*: Node " + str(line[2]) + " is removed")
            NODELISTING()
        elif line[1] == "SEND":
            print(
                "\tCOMMAND *SEND*: Data is ready to send from "
                + str(line[2])
                + " to "
                + str(line[3])
            )
            SEND(line[2], line[3], line[4])
# print(time_vortex)
"""print(node_dict)"""
"""
AdHocSim: A program that simulates Ad-Hoc Network.

Program starts just after network establishment and 
find routes using rucursions finally chooses the best 
one as optimal route.

# prints this message just after evaluation of CRNODE command


# prints this message just after evaluation of SEND command


# prints this message just after evaluation of MOVE command


# prints this message just after evaluation of RMNODE command


# prints this message just after evaluation of CHBTTRY command


# print this message every time a node is added/removed/relocated or battery of it is modified


# when there is no route found between node A and B


# prints number of possible routes found


# prints every route with its cost in a format given in the assignment paper


# prints optimal route with its cost in a format given in the assignment paper


# prints packet ID that is just sent

#prints remaining data size in format



# prints these labels at the end of output
"""
print("******************************")
print("AD-HOC NETWORK SIMULATOR - END")
print("******************************")
