/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

#define MAX_PROC 1000
#define NAME_SIZE 100

int main(int argc, char *argv[], char *environ[])
{

    int count = 0;
    int pcount;
    int child_status;
    pid_t pid[MAX_PROC], rpid;
    char a1[NAME_SIZE], code[NAME_SIZE], name[NAME_SIZE];

    sem_t *init[MAX_PROC], *go[MAX_PROC], *term[MAX_PROC], *full[MAX_PROC], *empty[MAX_PROC], *memlock[MAX_PROC];
    sem_t *memaccess;
    sem_t *bridge[MAX_PROC][MAX_PROC];

    pcount = atoi(argv[1]); // Get the number of processes

    for (int i = 0; i < pcount; i++)
    {
        sprintf(name, "mpi_init%d", i);
        init[i] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(init[i], 1, 0);

        sprintf(name, "go%d", i);
        go[i] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(go[i], 1, 0);

        sprintf(name, "termin%d", i);
        term[i] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(term[i], 1, 0);

        sprintf(name, "full%d", i);
        full[i] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(full[i], 1, 0);

        sprintf(name, "empty%d", i);
        empty[i] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(empty[i], 1, 0);

        sprintf(name, "memlock%d", i);
        memlock[i] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(memlock[i], 1, 1);
    }

    for (int i = 0; i < pcount; i++)
    {
        for (int a = 0; a < pcount; a++)
        {
            sprintf(name, "bridge%d%d", i, a); // Bridges like bridge00 or bridge22 are not used however if it were to be it does not break the code
            bridge[i][a] = sem_open(name, O_CREAT, 0600, 0);
            sem_init(bridge[i][a], 1, 0);
        }
    }

    sprintf(name, "memaccess");
    memaccess = sem_open(name, O_CREAT, 0600, 0);
    sem_init(memaccess, 1, 1);

    while (count < pcount)
    {
        if (pid[count] = fork() == 0)
        { // fork a new process
            strcpy(code, argv[2]);
            argv[0] = argv[1]; // Set nproc
            sprintf(a1, "%d", count);
            argv[1] = a1; // Set rank
            argv[2] = 0;
            execve(code, argv, environ); // Run MPI process
        }
        count++;
    }

    for (int i = 0; i < pcount; i++)
        sem_wait(init[i]); // Wait for each process to call MPI_Init

    for (int i = 0; i < pcount; i++)
        sem_post(go[i]); // Wait for each process to complete its MPI_Init tasks (for safe initialization of data structures, semaphores, etc.)

    for (int i = 0; i < pcount; i++)
        sem_wait(term[i]); // Wait for each process to call MPI_Finalize

    for (int i = 0; i < pcount; i++)
    {
        rpid = waitpid(pid[i], &child_status, WUNTRACED); // Reap the processes one by one
        sem_close(init[i]);
        sem_close(go[i]);
        sem_close(term[i]);
        sem_close(full[i]);
        sem_close(empty[i]);
        sem_close(memlock[i]);
    }

    for (int i = 0; i < pcount; i++)
    {
        for (int a = 0; a < pcount; a++)
        {
            sem_close(bridge[i][a]);
        }
    }

    sem_close(memaccess);

    return 0;
}