# Hacettepe University
# Kayla Akyüz
# 21726914

import sys

"""
This program will save the human race if done properly, 
so please do your best to get 100 from this assignment. 
You can do it! :)
"""

# Opening the input files

hurap_file = open(sys.argv[1], "r")

schuckscii_file = open(sys.argv[2], "r")

virus_codes_file = open(sys.argv[3], "r")

datc = {}
dath = []
dati = {}
linln = []
shift = ()


def split_every(n, s):
    return [s[i : i + n] for i in range(0, len(s), n)]


def htob(hx):
    if hx == "0":
        return "0000"
    if hx == "1":
        return "0001"
    if hx == "2":
        return "0010"
    if hx == "3":
        return "0011"
    if hx == "4":
        return "0100"
    if hx == "5":
        return "0101"
    if hx == "6":
        return "0110"
    if hx == "7":
        return "0111"
    if hx == "8":
        return "1000"
    if hx == "9":
        return "1001"
    if hx == "A":
        return "1010"
    if hx == "B":
        return "1011"
    if hx == "C":
        return "1100"
    if hx == "D":
        return "1101"
    if hx == "E":
        return "1110"
    if hx == "F":
        return "1111"


def bincon(binv):
    if binv == "0000":
        return "0"
    if binv == "0001":
        return "1"
    if binv == "0010":
        return "2"
    if binv == "0011":
        return "3"
    if binv == "0100":
        return "4"
    if binv == "0101":
        return "5"
    if binv == "0110":
        return "6"
    if binv == "0111":
        return "7"
    if binv == "1000":
        return "8"
    if binv == "1001":
        return "9"
    if binv == "1010":
        return "A"
    if binv == "1011":
        return "B"
    if binv == "1100":
        return "C"
    if binv == "1101":
        return "D"
    if binv == "1110":
        return "E"
    if binv == "1111":
        return "F"


for line in hurap_file:
    if line[:1] == "0":
        linln.insert(0, 1)
        datc[len(linln)] = line.rstrip("\n")
    elif line[:1] == "1":
        linln.insert(0, 1)
        datc[len(linln)] = line.rstrip("\n")
    else:
        shift = line
        continue

holdhex = []
hln = []
thmn = []
dathp = []
dathc = []
shcod = 0
shn = []
shift = [yasda for yasda in shift[1:].strip("\n")]
for inchr in shift[::-1]:
    for i in shift[:1]:
        determinator = i
    if int(determinator) == 0:
        inchr = int(inchr)
        shcod = shcod + inchr * (2 ** len(shn))
        shn.insert(0, 1)
    elif int(determinator) == 1:
        if len(shn) != len(shift[1:]):
            inchr = int(inchr)
            shcod = shcod + inchr * (2 ** len(shn))
            shn.insert(0, 1)
        elif len(shn) == len(shift[1:]):
            inchr = int(inchr)
            shcod = shcod - inchr * (2 ** len(shn))
            shn.insert(0, 1)

alen = []

for ind in range(1, len(linln) + 1):
    thmn.insert(0, 1)
    indil = str(datc.get(ind))
    indl = split_every(4, indil)
    for binc in indl:
        binc = bincon(binc)
        hln.insert(0, 1)
        if (len(hln)) == 1:
            holdhex.insert(len(holdhex), binc)
        elif (len(hln)) == 2:
            holdhex.insert(len(holdhex), binc)
            holdhex = "".join(holdhex)
            dath.insert(len(dath), (holdhex))
            hln = []
            holdhex = []
    dathc.insert(len(dathc), dath)
    dathp.insert(len(dathp), "".join(dath))
    dath = []

# Mission 00: Decrypting the HuRAP

print(
    """*********************
     Mission 00 
*********************""",
    end="\n\n",
)

print(
    """--- hex of encrypted code ---
-----------------------------""",
    end="\n\n",
)

print("\n".join(dathp))

# Your code which calculates and prints out the hexadecimal representation
# of HuRAP goes here

print(
    """\n--- encrypted code ----
-----------------------""",
    end="\n\n",
)

deth = {}
dethcl = []
dathe = {}
encp = []
encd = []
encdp = []

for line in schuckscii_file:
    alen.insert(0, 1)
    line = line
    dethcl.insert(0, 1)
    if not line:
        continue
    vline = line[2:4]
    dathe[line[0]] = vline
    deth[int(len(dethcl))] = line[0]

dathec = dict((v, k) for k, v in dathe.items())
dethr = dict((v, k) for k, v in deth.items())

shcod = shcod % len(alen)

for cnvr in dathc:
    for cnvrf in cnvr:
        cnvrf = str(cnvrf)
        bodsa = dathec[cnvrf]
        encd.insert(len(encd), bodsa)
    encdp.insert(len(encdp), "".join(encd))
    encp.insert(len(encp), encd)
    encd = []
print("\n".join(encdp))

# Your code which calculates and prints the encrypted character
# representation of HuRAP goes here

print(
    """\n--- decrypted code ---
----------------------""",
    end="\n\n",
)

decr = []
decrp = []
decc = []
for letl in encp:
    for letr in letl:
        thid = dethr[letr]
        thid = int(thid)
        if thid - shcod > len(alen):
            nwid = thid - shcod - len(alen)
        elif thid - shcod < 0:
            nwid = thid - shcod + len(alen)
        elif thid - shcod == 0:
            nwid = thid - shcod + len(alen)
        elif thid - shcod > 0:
            nwid = thid - shcod
        nltr = deth[nwid]
        decr.insert(len(decr), nltr)
        decc.insert(len(decc), nltr)
    decrp.insert(len(decrp), "".join(decr))
    decr = []

print("\n".join(decrp))

# Your code which decrypts and prints the
# HuRAP goes here

# Mission 01: Coding the virus

print(
    """\n*********************
     Mission 01 
*********************""",
    end="\n\n",
)

decrpm = decrp
vcod = {}

for line in virus_codes_file:
    line = line.rstrip("\n").split(":")
    if not line:
        continue
    vcod[line[0]] = line[1]

for eln in vcod:
    for iid, atid in enumerate(decrpm):
        natid = atid.replace(eln, (vcod[eln]))
        if natid != atid:
            decrpm.remove(atid)
            decrpm.insert(iid, natid)
        else:
            continue

print("\n".join(decrpm))

# Your code which transforms the original HuRAP and prints
# the virus-infected HuRAP goes here
# Mission 10: Encrypting the virus-infected HuRAP

print(
    """\n*********************
     Mission 10 
*********************""",
    end="\n\n",
)

print(
    """--- encrypted code ---
----------------------""",
    end="\n\n",
)

decr2 = []
decc2 = []
encrp = []

for letl2 in decrpm:
    letl2 = list(letl2)
    for letr2 in letl2:
        thid2 = dethr[letr2]
        thid2 = int(thid2)
        if thid2 + shcod > len(alen):
            nwid2 = thid2 + shcod - len(alen)
        elif thid2 + shcod < 0:
            nwid2 = thid2 + shcod + len(alen)
        elif thid2 + shcod == 0:
            nwid2 = thid2 + shcod + len(alen)
        else:
            nwid2 = thid2 + shcod
        nltr2 = deth[nwid2]
        decr2.insert(len(decr2), nltr2)
        decc2.insert(len(decc2), nltr2)
    encrp.insert(len(encrp), "".join(decr2))
    decr2 = []
print("\n".join(encrp))

# Your code which encrypts and prints the encrypted character
# representation of the virus-infected HuRAP goes here

print(
    """\n--- hex of encrypted code ---
-----------------------------""",
    end="\n\n",
)

hexg = []
hexgp = []

for linn in encrp:
    linn = list(linn)
    for letrr in linn:
        letrr = dathe[letrr]
        hexg.insert(len(hexg), letrr)
    hexgp.insert(len(hexgp), "".join(hexg))
    hexg = []
print("\n".join(hexgp))

# Your code which calculates and prints out the hexadecimal representation
# of virus-infected and encrypted HuRAP goes here

print(
    """\n--- bin of encrypted code ---
-----------------------------""",
    end="\n\n",
)
fl1 = []
flp = []
for flin in hexgp:
    flin = list(flin)
    for flet in flin:
        flet = htob(flet)
        fl1.insert(len(fl1), flet)
    flp.insert(len(flp), "".join(fl1))
    fl1 = []
print("\n".join(flp))

# Your code which calculates and prints out the binary representation
# of virus-infected and encrypted HuRAP goes here

# Closing the input files

hurap_file.close()
schuckscii_file.close()
virus_codes_file.close()
