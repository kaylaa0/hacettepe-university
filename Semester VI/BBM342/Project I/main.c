/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

#include <stdio.h> // Including libraries
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
int __nproc, __rank;

sem_t *init, *go, *term, *full, *empty, *memlock, *memaccess;
int shm_fd;
struct Shared_mem *pstart;
struct stat finfo;

int MPI_Init(int *argc, char ***argv)
{
    char name[20];

    __nproc = atoi((*argv)[0]);
    __rank = atoi((*argv)[1]);

    sprintf(name, "mpi_init%d", __rank);
    init = sem_open(name, 0);

    sprintf(name, "go%d", __rank);
    go = sem_open(name, 0);

    sprintf(name, "termin%d", __rank);
    term = sem_open(name, 0);

    sprintf(name, "full%d", __rank);
    full = sem_open(name, 0);

    sprintf(name, "empty%d", __rank);
    empty = sem_open(name, 0);

    sprintf(name, "memlock%d", __rank);
    memlock = sem_open(name, 0);

    sprintf(name, "memaccess");
    memaccess = sem_open(name, 0);

    sem_post(init);

    sem_wait(memaccess);

    shm_fd = shm_open("shared_memory", O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0)
    {
        printf("Error XYZ1\n");
        exit(0);
    }

    fstat(shm_fd, &finfo);
    off_t filesize = finfo.st_size;
    if (filesize == 0)
    { // This needs to be done only once so semaphore memaccess used to ensure there are no issues
        ftruncate(shm_fd, sizeof(int) * __nproc * 2);
    }
    pstart = mmap(NULL, sizeof(int) * __nproc * 2, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (MAP_FAILED == pstart)
    {
        printf("Error XYZ2\n");
        exit(0);
    }

    sem_post(memaccess);

    sem_wait(go);

    return 1;
}

int MPI_Comm_size(int *size)
{
    *size = __nproc;
    return 1;
}

int MPI_Comm_rank(int *rank)
{
    *rank = __rank;
    return 1;
}

int MPI_Finalize()
{

    sem_close(init); // Clearing semaphores
    sem_close(go);
    sem_close(full);
    sem_close(empty);
    sem_close(memlock);
    sem_close(memaccess);

    munmap(pstart, sizeof(int) * __nproc * 2); // Unlinking shared Memory
    shm_unlink("shared_memory");

    sem_post(term);
    sem_close(term);
    exit(0);
}

int MPI_Send(const void *buf, int count, int datatype, int dest, int tag)
{
    char name[20];
    sem_t *pipeBridge, *pipeBridge2;

    sprintf(name, "bridge%d%d", __rank, dest);
    pipeBridge = sem_open(name, 0);

    sprintf(name, "bridge%d%d", dest, __rank);
    pipeBridge2 = sem_open(name, 0);

    for (int i = 0; i < count; i++)
    {                          // Sending data count times.
        sem_wait(pipeBridge);  // Waiting for receiver
        sem_wait(memlock);     // Locking memory partition, sender's memory partition
        sem_post(pipeBridge2); // Signaling receiver that memory is locked
        // Debugging line.
        // printf("Hey %d sending %d to %d via %s\n", __rank, *((int *)(buf)), dest, name);
        memcpy((int *)((unsigned char *)pstart + ((datatype * 2) * __rank)), buf, datatype);               // Putting data in to memory
        memcpy((int *)((unsigned char *)pstart + ((datatype * 2) * __rank) + (datatype)), &tag, datatype); // Putting tag in to memory
        sem_post(full);                                                                                    // Signaling receiver that memory is full
        sem_wait(empty);                                                                                   // Waiting memory data to be consumed
    }

    sem_close(pipeBridge); // Closing semaphores
    sem_close(pipeBridge2);
    return 1;
}

int MPI_Recv(void *buf, int count, int datatype, int source, int tag)
{
    char name[20];
    int senderTag;
    sem_t *targetFull, *targetEmpty, *targetMemlock, *pipeBridge, *pipeBridge2;

    sprintf(name, "empty%d", source);
    targetEmpty = sem_open(name, 0);

    sprintf(name, "full%d", source);
    targetFull = sem_open(name, 0);

    sprintf(name, "memlock%d", source);
    targetMemlock = sem_open(name, 0);

    sprintf(name, "bridge%d%d", source, __rank);
    pipeBridge = sem_open(name, 0);

    sprintf(name, "bridge%d%d", __rank, source);
    pipeBridge2 = sem_open(name, 0);

    for (int i = 0; i < count; i++)
    {
        sem_post(pipeBridge);                                                                                    // Signaling sender that receiver is here, they will lock memory
        sem_wait(pipeBridge2);                                                                                   // Waiting sender to lock memory
        sem_wait(targetFull);                                                                                    // Waiting sender to write data
        memcpy(&senderTag, (int *)((unsigned char *)pstart + ((datatype * 2) * source) + (datatype)), datatype); // Reading tag
        if (senderTag == tag)
        {                                                                                        // If tag equals
            memcpy(buf, (int *)((unsigned char *)pstart + ((datatype * 2) * source)), datatype); // Reading data to buffer
        }
        else
        {
            int error = -1;
            printf("Tag mismatch between rank (receiver): %d and (sender): %d , receiver tag: %d , sender tag: %d\n", __rank, source, tag, senderTag);
            memcpy(buf, &error, datatype); // This is to ensure dev testing does not do segmentation fault, -1 is written in buffer
            printf("Error data '-1' has been written to buffer since tag mismatch.\n");
        }
        sem_post(targetMemlock); // Unlocking memory
        sem_post(targetEmpty);   // Signaling sender that data is consumed
    }

    // Debugging line.
    // printf("Hey!! %d received %d from %d via %s\n", __rank, *((int *)(buf)), source, name);

    sem_close(targetEmpty); // Closing semaphores
    sem_close(targetFull);
    sem_close(pipeBridge);
    sem_close(pipeBridge2);
    return 1;
}

int main(int argc, char *argv[], char *environ[])
{

    int npes, myrank, number, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(&npes);
    MPI_Comm_rank(&myrank);

    if (myrank == 0)
    {

        for (i = 1; i < npes; i++)
        {
            MPI_Recv(&number, 1, sizeof(int), i, 0);
            printf("From process %d  data= %d, RECEIVED!\n", i, number);
        }
    }
    else
    {
        MPI_Send(&myrank, 1, sizeof(int), 0, 0);
    }

    if (myrank == 0)
    {
        for (i = 1; i < npes; i++)
        {
            MPI_Send(&i, 1, sizeof(int), i, 0);
        }
    }
    else
    {
        MPI_Recv(&number, 1, sizeof(int), 0, 0);
        printf("RECEIVED from %d data= %d, pid=%d \n", myrank, number, getpid());
    }

    printf("STRESS TEST  %d\n", myrank);

    for (i = 0; i < 100000; i++) // 100000
    {
        if (myrank % 2 == 0)
        {
            MPI_Recv(&number, 1, sizeof(int), (myrank + 1) % npes, 0);
            MPI_Send(&number, 1, sizeof(int), (myrank + 1) % npes, 0);
        }
        else
        {
            MPI_Send(&number, 1, sizeof(int), (myrank - 1 + npes) % npes, 0);
            MPI_Recv(&number, 1, sizeof(int), (myrank - 1 + npes) % npes, 0);
        }
    }

    printf("STRESS TEST FINISHED  %d\n", myrank);

    MPI_Finalize();
}
