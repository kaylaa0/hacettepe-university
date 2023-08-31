# -*- MakeFile -*-
CFLAGS = -std=c99 -D_POSIX_C_SOURCE=200809L

BNF: BNF.o
	gcc BNF.o -o BNF

BNF.o: BNF.h BNF.c
	gcc $(CFLAGS) -c BNF.c