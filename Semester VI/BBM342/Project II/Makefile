# -*- MakeFile -*-

all: sserver

sserver: sserver.o ; gcc sserver.o -o sserver -lpthread -lrt

sserver.o: sserver.c ; gcc -c sserver.c -std=c99 -g -D_XOPEN_SOURCE=500

clean: ; -rm -f hello