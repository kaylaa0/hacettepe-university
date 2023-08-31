# -*- MakeFile -*-

all: hello my_mpirun

hello: main.o ; gcc main.o -o hello -lpthread -lrt

main.o: main.c ; gcc -c main.c -std=c99 -g -D_XOPEN_SOURCE=500

my_mpirun: mpirun.o ; gcc mpirun.o -o my_mpirun -lpthread -lrt

mpirun.o: mpirun.c ; gcc -c mpirun.c -std=c99 -g -D_XOPEN_SOURCE=500

clean: ; -rm -f hello ; -rm -f my_mpirun