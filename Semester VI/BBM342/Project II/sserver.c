/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

// I tried to use puts on runtime critical points.
// Considering debugging sometimes printf is used to get values.
// If data > time cost I used printf.
// Also there are debugging prints. You can uncomment and use them or just ignore them.

// You can close the server by typing "close" or "exit" or "quit" or "server_close" or "server_exit" or "server_quit" in console. This method is suggested when closing.
// Otherwise the shared memory and semaphores will stay open.
// My program is tested for memory leaks. Result: No memory leak.

// Up to 300 clients per channel.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>

// Change this values accordingly.
// YOU MUST CHANGE THEM IF YOU CHANGE INPUT FILE
// ESPECIALLY IF HEIGHT AND WIDTH CHANGED ETC.
#define MAX_PROC 908       // Maximum threads, 1 server + 1 input listener + 3 producer + 3 channels + n workers
#define MAX_CLIENT_PER 300 // Maximum client per channel. Aka workers per channel. 300 is a lot and enough.
// Because of 300 per channel it will some time to create and run and take a little bit of space. 300 is a lot but assuring.
#define STREAM_HEIGHT 13 // Stream art line number for one frame
#define STREAM_WIDTH 67  // Stream art character number for one line
#define TARGET_FPS 20    // Target fps
#define BUFFER_SECOND 5  // Buffered second from producers to channels, 5 is enough buffer. There is some work done so set this a little bit higher.

// Thread data struct.
// This is only used for workers.
// Only workers are threaded.
// Rest are child aka forked.
struct thread_data
{
    int clientSock;
    int channel;
    int workerSlot;
};

// Keeping record of threads here.
pthread_t thread_id[9000]; // Maximum workers * times. This does not get reduced. Always records. So even if you worker disconnects record remain. So we need big value if this fills we can not record threads meaning fatal.
int current_thread_id = 0; // Keep our thread record.

// Yes this seems noob but it was needed.
int one = 1;
int zero = 0;

// Saving our pids.
// Server pid is server pid known from everyone forked.
pid_t ServerPID;
// Channel pids are channel pids. Used when terminating.
pid_t ChannelPIDs[3];

// This is server status.
// When close command is run the command listener sends signals SIGUSR1 accordingly to all other threads.
// And they set their server_status false.
// Breaking their job, loops and entering in to closing state.
int server_status;

// These are special set of variables for this situation:
// Lets say channel is being setup but the producer has invalid file name or invalid file etc.
// Producer closes.
// Channel remains sad.
// This set of variables and how they are used ensure it never happens.
// When error above happens the producer sends SIGUSR2 signal to parent. This signal is for this error.
// Also writes its number to a memory space.
// Parent handles signal:
// Reads memory space.
// Sends SIGUSR1 (server termination signal) to according channel.
// Channels never wait for invalid producers they close.
int producer_one_error = 0;
int producer_two_error = 0;
int producer_three_error = 0;

int channel_one_init = 0;
int channel_two_init = 0;
int channel_three_init = 0;

// Semaphores for channel buffer.
// It is big buffer.
// Producer consumer method.
// CHANNEL BUFFER IS NOT MEM LOCKED
// THIS WAY IT IS FASTER
// IT DOES NOT NEED MEM LOCK
// CHANNEL NEVER CATCHES THE PRODUCER BUFFER
sem_t *channel_buffer_mem_remaining[3], *channel_buffer_mem_written[3];
// Memaccess semaphore for server_status value shmem.
// server_status used to close the server and childs and threads.
sem_t *status_memaccess;
// Memaccess semaphore for SIGUSR2 value shmem.
// SIGUSR2 used to notice server if a producer crashes like; invalid file.
sem_t *SIGUSR2_memaccess;
sem_t *SIGUSR2_buffer;

// Used between worker and channel. Makes channel online and offline. If no worker channel idle, no work.
// If channel not consume -> producer consumer above does no work so producer no work.
// Meaning system idles when not used.
// Online system works this way:
// Channel online is 0, channel waits for it.
// Worker comes and makes it 1
// Channel online, channel posts it back after using so keeps open.
// If another worker comes it just increases.
// When last worker is gone it is back to 0.
// Channel waits.
// Now problem rises.
// While worker waits, channel might, slight chance, but if fast enough, who knows, can post and consume its own signal back.
// Worker can never take away what it gave.
// Worker can not close channel runs like maniac.
// So this online value is locked by a mutex.
// When worker waits for online channel can not wait for it. Worker will get it for sure because it is in mutex lock.
// Nice!!
sem_t *channel_online[3];
sem_t *channel_mutex[3];

// Worker and channels memory space are used in producer consumer method. However is not buffered. It is set to 1.
// This is just to ensure the worker reads the value and we don't overwrite.
// Not a chance to happen since there are up to 300 workers.
// But who knows channel can be really fast.
sem_t *worker_producers[MAX_CLIENT_PER * 3];
sem_t *worker_consumers[MAX_CLIENT_PER * 3];

// Memaccess for above structure
sem_t *worker_memaccess[MAX_CLIENT_PER * 3];

// Channel worker is the vector 3->300 where worker status is hold.
// This is apart from channel online.
// When channel becomes online it also knows which workers are here.
// Workers register in like a factory they use shared memory and they put 1 to their slot.
// This is important since their slot is unique.
// It gets freed automatically if worker leaves.
// Worker gets assigned slot before it connects.
// If no slot left meaning channel has 300 workers !!! WOW
// It won't start, it will error client and close.
// Channel will send data to all of the currently online worker slots.
// Meaning it will check all 300 regardless.
// This could be done in some smart way like:
// Channel knows how many workers and does not look more when it is done.
// However it also has its time costs and draw backs so not used.
sem_t *channel_worker_memaccess;

// Shared memories used for descriptions above.
int shm_channel_worker; // worker channel vector 3->300
struct Shared_mem *shm_channel_worker_start;
struct stat channel_worker_finfo;

int shm_worker[MAX_CLIENT_PER * 3]; // workers own memory space. Rather slot's memory space. Workers come and go. Slot remains and assigned to new worker.
struct Shared_mem *shm_worker_start[MAX_CLIENT_PER * 3];
struct stat worker_finfo[MAX_CLIENT_PER * 3];

int shm_status; // Status of server, only 1 int.
struct Shared_mem *shm_status_start;
struct stat status_finfo;

int shm_channel_buffer[3]; // Channel buffer, is big!
struct Shared_mem *shm_channel_buffer_start[3];
struct stat buffer_finfo[3];

int shm_SIGUSR2; // Shared memory for SIGUSR2, SIGUSR2 used for producer errors. shared memory is to know which producer gave error.
struct Shared_mem *shm_SIGUSR2_start;
struct stat SIGUSR2_finfo;

struct Shared_mem *create_shared_memory(char *memory_name, int shm_fd, struct Shared_mem *shm_start, struct stat finfo, int size)
{
    // Since so many shared memories, they are created with a function smartly.
    shm_fd = shm_open(memory_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0)
    {
        printf("Error XYZ1\n");
        exit(0);
    }
    fstat(shm_fd, &finfo);
    off_t filesize = finfo.st_size;

    if (filesize == 0)
    { // Allocating space for shared memory
        ftruncate(shm_fd, size);
    }

    shm_start = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (MAP_FAILED == shm_start)
    {
        printf("Error XYZ2\n");
        exit(0);
    }
    return shm_start;
}

int server_init(int *argc, char ***argv)
{
    // Init of server, all semaphores and shared memory initialized.
    // Look above for how all these used.
    char name[40];
    sprintf(name, "status_memaccess");
    status_memaccess = sem_open(name, O_CREAT, 0600, 0);
    sem_init(status_memaccess, 1, 1);

    sprintf(name, "channel_worker_memaccess");
    channel_worker_memaccess = sem_open(name, O_CREAT, 0600, 0);
    sem_init(channel_worker_memaccess, 1, 1);

    sprintf(name, "SIGUSR2_memaccess");
    SIGUSR2_memaccess = sem_open(name, O_CREAT, 0600, 0);
    sem_init(SIGUSR2_memaccess, 1, 1);

    sprintf(name, "SIGUSR2_buffer");
    SIGUSR2_buffer = sem_open(name, O_CREAT, 0600, 0);
    sem_init(SIGUSR2_buffer, 1, 1);

    for (int channelNo = 1; channelNo <= 3; channelNo++)
    {
        sprintf(name, "channel_buffer_mem_remaining_%d", channelNo);
        channel_buffer_mem_remaining[channelNo - 1] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(channel_buffer_mem_remaining[channelNo - 1], 1, (TARGET_FPS * BUFFER_SECOND));

        sprintf(name, "channel_buffer_mem_written_%d", channelNo);
        channel_buffer_mem_written[channelNo - 1] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(channel_buffer_mem_written[channelNo - 1], 1, 0);

        sprintf(name, "channel_online_%d", channelNo);
        channel_online[channelNo - 1] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(channel_online[channelNo - 1], 1, 0);

        sprintf(name, "channel_mutex_%d", channelNo);
        channel_mutex[channelNo - 1] = sem_open(name, O_CREAT, 0600, 0);
        sem_init(channel_mutex[channelNo - 1], 1, 1);

        sprintf(name, "shm_channel_buffer_%d", channelNo);
        shm_channel_buffer_start[channelNo - 1] = create_shared_memory(name, shm_channel_buffer[channelNo - 1], shm_channel_buffer_start[channelNo - 1], buffer_finfo[channelNo - 1], sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH * TARGET_FPS * BUFFER_SECOND);

        for (int workerNo = 1; workerNo <= MAX_CLIENT_PER; workerNo++)
        {
            sprintf(name, "worker_producer%dof%d", workerNo, channelNo);
            worker_producers[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)] = sem_open(name, O_CREAT, 0600, 0);
            sem_init(worker_producers[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)], 1, 0);

            sprintf(name, "worker_consumer%dof%d", workerNo, channelNo);
            worker_consumers[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)] = sem_open(name, O_CREAT, 0600, 0);
            sem_init(worker_consumers[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)], 1, 1);

            sprintf(name, "worker_memory%dof%d", workerNo, channelNo);
            worker_memaccess[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)] = sem_open(name, O_CREAT, 0600, 0);
            sem_init(worker_memaccess[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)], 1, 1);

            sprintf(name, "shm_worker_%d_%d", channelNo, workerNo);
            shm_worker_start[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)] = create_shared_memory(name, shm_worker[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)], shm_channel_buffer_start[channelNo - 1], worker_finfo[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)], sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
        }
    }

    shm_channel_worker_start = create_shared_memory("shared_channel_worker_memory", shm_channel_worker, shm_channel_worker_start, channel_worker_finfo, sizeof(int[MAX_CLIENT_PER]) * 3);

    shm_status_start = create_shared_memory("shared_status_memory", shm_status, shm_status_start, status_finfo, sizeof(int));

    shm_SIGUSR2_start = create_shared_memory("shm_SIGUSR2_memory", shm_SIGUSR2, shm_SIGUSR2_start, SIGUSR2_finfo, sizeof(int));

    // When initialized we make server shared memory one.
    memcpy(shm_status_start, &one, sizeof(int));
    // And read it back.
    // Kinda a way to know everything is allright.
    memcpy(&server_status, (int *)shm_status_start, sizeof(int));

    // :) I even checked it just to be sure.
    // printf("%d \n", server_status);
    return 1;
}

int close_and_free()
{
    // This is closing semaphores.
    // When a child closes or obviously if server closes this is called.
    // Not called for threads :) since that is the way.
    sem_close(status_memaccess);
    sem_close(channel_worker_memaccess);
    sem_close(SIGUSR2_memaccess);
    sem_close(SIGUSR2_buffer);

    for (int channelNo = 1; channelNo <= 3; channelNo++)
    {
        sem_close(channel_buffer_mem_remaining[channelNo - 1]);
        sem_close(channel_buffer_mem_written[channelNo - 1]);
        sem_close(channel_online[channelNo - 1]);
        sem_close(channel_mutex[channelNo - 1]);
        for (int workerNo = 1; workerNo <= MAX_CLIENT_PER; workerNo++)
        {
            sem_close(worker_producers[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)]);
            sem_close(worker_consumers[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)]);
            sem_close(worker_memaccess[((channelNo - 1) * MAX_CLIENT_PER) + (workerNo - 1)]);
        }
    }
    return 0;
}

int server_close()
{
    // Server close
    // First closing semaphores
    close_and_free();

    // Freeing memories since we dont need it.
    munmap(shm_channel_worker_start, sizeof(int[MAX_CLIENT_PER]) * 3); // Unlinking shared Memory
    shm_unlink("shared_channel_worker_memory");

    munmap(shm_status_start, sizeof(int)); // Unlinking shared Memory
    shm_unlink("shared_status_memory");

    char name[40];
    for (int i = 0; i < 3; i++)
    {
        sprintf(name, "shm_channel_buffer_%d", i + 1);
        munmap(shm_channel_buffer_start[i], sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH * TARGET_FPS * BUFFER_SECOND); // Unlinking shared Memory
        shm_unlink(name);

        for (int workerNo = 1; workerNo <= MAX_CLIENT_PER; workerNo++)
        {
            sprintf(name, "shm_worker_%d_%d", i + 1, workerNo);
            munmap(shm_worker_start[((i)*MAX_CLIENT_PER) + (workerNo - 1)], sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
            shm_unlink(name);
        }
    }

    munmap(shm_SIGUSR2_start, sizeof(int)); // Unlinking shared Memory
    shm_unlink("shm_SIGUSR2_memory");

    // Printing the final.
    puts("Server closed.");
    exit(0);
}

int child_close()
{
    // Same as server_close only difference is print to know what is closed and memories are not freed obviously.
    close_and_free();

    puts("Child closed.");
    exit(0);
}

int produced_failed_to_init(int producerNO)
{
    // If producer failed to init it will send SIGUSR2 to parent (server).
    // It will also write its not to shared memory.
    sem_wait(SIGUSR2_buffer);
    sem_wait(SIGUSR2_memaccess);
    memcpy((int *)shm_SIGUSR2_start, &producerNO, sizeof(int));
    sem_post(SIGUSR2_memaccess);
    kill(ServerPID, SIGUSR2); // Sending signal.
    child_close();
}

void receive_producer_data()
{
    // Producer closed function handling from server.
    // Gets called from SIGUSR2 Handler.
    // Reads memory of which producer errored.
    int erroredProducer;
    sem_wait(SIGUSR2_memaccess);
    memcpy(&erroredProducer, (int *)shm_SIGUSR2_start, sizeof(int));
    sem_post(SIGUSR2_memaccess);
    printf("Producer %d is errored. \n", erroredProducer); // Prints
    // Sets errored producer value so we know it errored when we go back to execution
    if (erroredProducer == 1)
    {
        // If channel was initialized we call SIGUSR1 to close it.
        if (channel_one_init == 1)
        {
            kill(ChannelPIDs[0], SIGUSR1);
        }
        // Channel is not init anymore
        // This way when a client come for that channel we say no sorry it is closed.
        channel_one_init = 0;
        // Producer one error is tracked in server.
        producer_one_error = 1;
    }
    else if (erroredProducer == 2)
    {
        if (channel_two_init == 1)
        {
            kill(ChannelPIDs[1], SIGUSR1);
        }
        channel_two_init = 0;
        producer_two_error = 1;
    }
    else if (erroredProducer == 3)
    {
        if (channel_three_init == 1)
        {
            kill(ChannelPIDs[2], SIGUSR1);
        }
        channel_three_init = 0;
        producer_three_error = 1;
    }
    sem_post(SIGUSR2_buffer);
}

static void parent_signal(int sig)
{
    // SIGUSR2
    // Server handler for SIGUSR2
    // Server handles producer error
    if (sig == SIGUSR2)
    { // Used for producer file read error etc.
        puts("Received SIGUSR2 error.");
        receive_producer_data(); // Calls the function.
        // Sets it back, we never know how many of them gonna fail :)
        signal(SIGUSR2, parent_signal);
        puts("Handled SIGUSR2 error.");
    }
}

void child_close_signal(int sig)
{
    // SIGUSR1
    // Handler for SIGUSR1
    // SIGUSR1 notices the child or thread or server that the server is up to close. Meaning close or exit or quit or server_close or server_exit or server_quit is typed to command line.
    if (sig == SIGUSR1)
    {
        puts("Received SIGUSR1 error.");
        signal(SIGUSR1, child_close_signal); // Set it back cuz it will be called again at the end just to be sure.
        server_status = 0;
    }
}

int server_morph_producer(int producerNo, char *ch1, char *ch2, char *ch3)
{
    // This is when a server is forked in to a child which is producer.
    // It will come to this function.
    // And become a producer.
    char readLine[STREAM_WIDTH]; // Line read from file.
    char *filename;              // Filename
    if (producerNo == 1)
    {
        // Producer 1 can only be ch1 file
        filename = ch1;
    }
    else if (producerNo == 2)
    {
        if (strcmp(ch2, "-1") != 0)
        {
            filename = ch2;
        }
        else
        {
            // This is not implemented but if you did not define channel 2-3 files, they will still be online streaming channel 1.
            filename = ch1;
            puts("Channel 2 file seems to not be specified. Will use channel 1's file for channel 2.");
        }
    }
    else if (producerNo == 3)
    {
        if (strcmp(ch3, "-1") != 0)
        {
            filename = ch3;
        }
        else
        {
            filename = ch1;
            puts("Channel 2 file seems to not be specified. Will use channel 1's file for channel 2.");
        }
    }
    else
    {
        puts("Invalid producer no");
        child_close();
    }

    if (strcmp((char *)&filename[(strlen(filename) - 4)], ".txt") != 0)
    {
        // Does not matter if you type .txt it will be handled.
        strcat(filename, ".txt");
        printf("Channel %d's file does not end with '.txt' . It will be added automatically don't worry.\n", producerNo);
    }

    // puts(filename);

    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL)
    {
        printf("Error! File cannot be opened. %s\n", filename);
        // Program exits if the file pointer returns NULL.
        // The SIGUSR2 to server will be called as soon as possible.
        produced_failed_to_init(producerNo);
        exit(1);
    }

    // So what happens down below is this:
    // We read line by line,
    // Populate a buffer.
    // If our line number mod STREAM HEIGHT+1 is 0 meaning we read a full frame
    // We put it to memory buffer.
    // Continue.
    // So one thing is handled:
    // If there is a number for time for a frame.
    // It will be put into buffer n times.
    // This was not necessary but it is how it should have been handled according to TA.
    // So when we finish file we loop.
    // There is a special fix.
    // When we populate memory we loop.
    // Memory size is consumer producer method locked meaning when we full we wait until something consumed from beginning and we loop and wait and etc.
    int currentLine = 0;
    int timeData = -1;
    char tempFrameBuffer[STREAM_WIDTH * STREAM_HEIGHT];
    int currentFrame = 0;
    while (server_status)
    {
        if (fgets(readLine, STREAM_WIDTH + 2, fptr) != NULL)
        {
            if (currentLine % (STREAM_HEIGHT + 1) == 0)
            {
                // Time Data
                // Printed n times meaning the number in file before every frame.
                while (timeData >= 1)
                {
                    // If time data bigger than one means we have a frame buffered so we put that frame timeData times to mem buffer.
                    sem_wait(channel_buffer_mem_remaining[producerNo - 1]); // PRODUCER CONSUMER METHOD
                    if (!server_status)
                    {
                        break;
                    }
                    memcpy((int *)((unsigned char *)shm_channel_buffer_start[producerNo - 1] + ((currentFrame)*STREAM_HEIGHT * STREAM_WIDTH)), tempFrameBuffer, sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
                    sem_post(channel_buffer_mem_written[producerNo - 1]);
                    // If current frame is end of mem buffer we loop.
                    currentFrame++;
                    if (currentFrame >= TARGET_FPS * BUFFER_SECOND)
                    {
                        currentFrame = 0;
                    }
                    timeData--;
                }
                timeData = atoi(readLine);
                // We reset the buffer.
                strcpy(tempFrameBuffer, "");
            }
            else
            {
                // Frame Data
                strcat(tempFrameBuffer, readLine);
            }
            currentLine++;
        }
        else
        {
            // DEAR TA
            // THIS IS A FIX
            // FOR SOME REASON (I THINK SOMETHING WITH EOF'S) EVEN THO MY CODE SHOULD WORK NON-STOP
            // WHEN CLIENT RECEIVES LAST FRAME IT STOPS
            // WHAT IS SPECIAL ABOUT LAST FRAME ? NO THING!!
            // I THINK THERE IS HIDDEN EOF OR SOMETHING MISSING LIKE \n
            // THAT IS WHY THIS SOLUTION IMPLEMENTED LAST CHARACTER OF LAST LINE IS SET TO \n MANUALLY
            // TAKES TIME BUT ONLY WAY I COULD FIX DYNAMICALLY AFTER SPENDING ONE DAY ON THIS ISSUE
            if (!server_status)
            {
                break;
            }
            char temp[3000];
            memcpy(temp, (int *)((unsigned char *)shm_channel_buffer_start[producerNo - 1] + ((currentFrame)*STREAM_HEIGHT * STREAM_WIDTH)), sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
            temp[strlen(temp) - 1] = '\n';
            memcpy((int *)((unsigned char *)shm_channel_buffer_start[producerNo - 1] + ((currentFrame)*STREAM_HEIGHT * STREAM_WIDTH)), temp, sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);

            // Values are reset, file is rewinded.
            timeData = -1;
            rewind(fptr);
            currentLine = 0;
        }
    }
    // If SIGUSR1 is called and the producer is out of loop or get error.
    printf("Input producer child %d is closing. \n", producerNo);
    // Close file
    fclose(fptr);
    // Close producer.
    child_close();
    return 1;
}

int server_morph_channel(int channelNo)
{
    // This is when a server is forked in to a child which is channel.
    // It will come to this function.
    // And become a channel.
    char tempFrameBuffer[STREAM_WIDTH * STREAM_HEIGHT];
    int tempChannelWorker[MAX_CLIENT_PER * 3];
    int currentFrame = 0;
    // How it works; consumer producer with producer.
    // Reads and waits to be online
    // Online system described at semaphores. Check for details.
    // If online checks the vector, if it's own space slots is 1 meaning there is a worker in that spot,
    // Writes to mem space of that spot.
    // Continues until offline.
    if (channelNo < 4)
    { // Making sure channel no is not invalid.
        while (server_status)
        {
            sem_wait(channel_buffer_mem_written[channelNo - 1]);
            if (!server_status)
            {
                break;
            }
            memcpy(tempFrameBuffer, (int *)((unsigned char *)shm_channel_buffer_start[channelNo - 1] + ((currentFrame)*STREAM_HEIGHT * STREAM_WIDTH)), sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
            sem_post(channel_buffer_mem_remaining[channelNo - 1]);

            // Online system is mutex locked.
            // puts("Channel wait mutex");
            sem_wait(channel_mutex[channelNo - 1]);
            // puts("Channel wait online");
            sem_wait(channel_online[channelNo - 1]);
            sem_post(channel_mutex[channelNo - 1]);
            // Just sends it back, so it does not use it just checks if online is 1.
            // Worker is the decider here.
            sem_post(channel_online[channelNo - 1]);

            // Vector is memlocked cuz it can be clashed.
            sem_wait(channel_worker_memaccess);
            memcpy(tempChannelWorker, (int *)shm_channel_worker_start, sizeof(int[MAX_CLIENT_PER]) * 3);
            sem_post(channel_worker_memaccess);

            // For all slots in our area in vector.
            for (int currentSlotChecked = (channelNo - 1) * MAX_CLIENT_PER; currentSlotChecked < channelNo * MAX_CLIENT_PER; currentSlotChecked++)
            {
                if (tempChannelWorker[currentSlotChecked] == 1)
                {
                    // If a slot is 1 we wait for consumer it is default is 1.
                    // So if this is first time we just write.
                    // If not first time we make sure the worker has read data. Wait him.
                    // If a worker fails we wait and channel fails too.
                    // SO A CHANNEL MIGHT FAIL IN CASE OF WORKER FAILURE.
                    // It is bad but how it is.
                    printf("Waiting worker %d consumer at channel %d\n", currentSlotChecked - ((channelNo - 1) * (MAX_CLIENT_PER)), channelNo);
                    printf("Current frame: %d\n", currentFrame);
                    sem_wait(worker_consumers[currentSlotChecked]);

                    // Writing to its memory space.
                    sem_wait(worker_memaccess[currentSlotChecked]);
                    memcpy((int *)shm_worker_start[currentSlotChecked], tempFrameBuffer, sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
                    sem_post(worker_memaccess[currentSlotChecked]);
                    sem_post(worker_producers[currentSlotChecked]);
                }
            }
            // Looping the channel-producer buffer.
            currentFrame++;
            if (currentFrame >= TARGET_FPS * BUFFER_SECOND)
            {
                currentFrame = 0;
            }
        }
    }
    // Closing of channel.
    printf("Input channel child %d is closing. \n", channelNo);
    child_close();
    return 1;
}

void *server_thread_worker(void *threadData)
{
    // Workers are threads not childs.
    // Therefore they behave differently.
    // Get the socket descriptor
    struct thread_data *data = threadData; // Thread data is casted

    int sock = data->clientSock; // Reading thread datas
    int channel = data->channel;
    int worker_slot = data->workerSlot;

    int read_size;             // Variables used.
    char client_message[2000]; // Not needed to be this big since we only get pings but still.

    char frameData[STREAM_HEIGHT * STREAM_WIDTH];
    // The height data is send to client.
    // Only data it needs.
    // Rest is calculated here anyway.
    char tempNumString[10];
    sprintf(tempNumString, "%d\n", STREAM_HEIGHT); // MAKING SURE IT ENDS WITH \n CUZ WE READ LINE WRITE LINE SEND LINE

    while (server_status && ((read_size = recv(sock, client_message, 2000, 0)) > 0))
    {
        // Sending data to socket.
        write(sock, tempNumString, strlen(tempNumString));
        break;
    }
    // When we ready to stream we make channel online.
    // Channel was waiting for this semaphore.
    // We will take it back when we finish.
    // It is mutex locked.
    sem_post(channel_online[channel - 1]);
    printf("Slot %d of channel %d has become online on socket %d\n", worker_slot, channel, sock);

    while (server_status && ((read_size = recv(sock, client_message, 2000, 0)) > 0))
    {
        // We get ping -> We send frame. Basic stuff.
        puts("Sending");
        sem_wait(worker_producers[((channel - 1) * MAX_CLIENT_PER) + worker_slot]); // Waiting channel to produce our memspace.
        memcpy(frameData, (int *)shm_worker_start[((channel - 1) * MAX_CLIENT_PER) + worker_slot], sizeof(char) * STREAM_HEIGHT * STREAM_WIDTH);
        // SENDING FRAME WITH TCP

        write(sock, frameData, strlen(frameData));
        puts(frameData);

        // end of string marker
        // client_message[read_size] = '\0';
        // puts(client_message);
        // clear the message buffer
        //  Client message not used anyways.
        memset(client_message, 0, 2000);
        memset(frameData, 0, STREAM_WIDTH * STREAM_HEIGHT); // Clearing temp variable.
        // FINISHED SENDING FRAME
        sem_post(worker_consumers[((channel - 1) * MAX_CLIENT_PER) + worker_slot]);
    }

    // TCP Errors checked.
    if (read_size == 0)
    {
        puts("Client disconnected"); // If it connects we know.
        fflush(stdout);
    }

    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // Closing worker.
    puts("Worker Closing.");
    // First we set our slot to 0 so channel does not work for us.
    sem_wait(channel_worker_memaccess);
    int channel_worker_main[MAX_CLIENT_PER * 3];
    // Read it to temp and modify and memcpy back.
    memcpy(channel_worker_main, (int *)shm_channel_worker_start, sizeof(int[MAX_CLIENT_PER]) * 3);
    channel_worker_main[((channel - 1) * MAX_CLIENT_PER) + worker_slot] = 0;
    memcpy((int *)(unsigned char *)shm_channel_worker_start, channel_worker_main, sizeof(int[MAX_CLIENT_PER]) * 3);
    sem_post(channel_worker_memaccess);

    puts("Cleared worker slot memory.");
    // Worker is going to get channel online back.
    sem_wait(channel_mutex[channel - 1]);
    puts("Worker wait online.");
    sem_wait(channel_online[channel - 1]);
    sem_post(channel_mutex[channel - 1]);
    puts("Reduced channel online.");
    return 0;
}

int get_slot(int channelNo)
{
    // Function to get available slot from vector 3->300 channel worker.
    // We read and set available slot to 1 and memcpy it back too so channel also knows it.
    // Return too and returned value will be passed to worker too so it also knows it slot.
    // Max 300 slot per channel.
    int channel_worker_main[MAX_CLIENT_PER * 3];
    sem_wait(channel_worker_memaccess);
    // Access locked shared memory cuz it can clash!!!
    memcpy(channel_worker_main, (int *)shm_channel_worker_start, sizeof(int[MAX_CLIENT_PER]) * 3);
    for (int slot = (channelNo - 1) * MAX_CLIENT_PER; slot < (channelNo)*MAX_CLIENT_PER; slot++)
    {
        if (channel_worker_main[slot] == 0)
        {
            channel_worker_main[slot] = 1;
            memcpy((int *)(unsigned char *)shm_channel_worker_start, channel_worker_main, sizeof(int[MAX_CLIENT_PER]) * 3);
            sem_post(channel_worker_memaccess);
            // printf("Slot: %d \n",(slot-((channelNo-1)*MAX_CLIENT_PER)));
            return (slot - ((channelNo - 1) * MAX_CLIENT_PER));
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    // Main function
    // They are -1 so we know if thing go wrong.
    int port = -1;
    int stream_number = -1;
    char *video_file_one = "-1";
    char *video_file_two = "-1";
    char *video_file_three = "-1";
    // Reading arguments.
    for (int i = 1; i < argc; i++)
    {
        // puts(argv[i]);
        // Why double ? Because – (from project document) and - (normal middle line) are not equal. TA used a special line in document.
        // Just to make sure
        if (strcmp(argv[i], "–p") == 0)
        {
            port = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            port = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "–s") == 0)
        {
            stream_number = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            stream_number = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "–ch1") == 0)
        {
            video_file_one = argv[i + 1];
        }
        else if (strcmp(argv[i], "-ch1") == 0)
        {
            video_file_one = argv[i + 1];
        }
        else if (strcmp(argv[i], "–ch2") == 0)
        {
            video_file_two = argv[i + 1];
        }
        else if (strcmp(argv[i], "-ch2") == 0)
        {
            video_file_two = argv[i + 1];
        }
        else if (strcmp(argv[i], "–ch3") == 0)
        {
            video_file_three = argv[i + 1];
        }
        else if (strcmp(argv[i], "-ch3") == 0)
        {
            video_file_three = argv[i + 1];
        }
    }
    // Checking if arguments are valid.
    // I tried to code smart so I can catch if you make mistakes and warn you
    // But still please be careful.
    if (port == -1)
    {
        puts("Can not read port value from arguments make sure you use following configuration: ");
        puts("...    –p <port>");
        puts("Type port number (Ex:59898) instead of <port>, without angle brackets.");
        puts("Server will close port number must be specified.");
    }
    else if (stream_number == -1)
    {
        puts("Can not read stream number from arguments make sure you use following configuration: ");
        puts("...    –s <stream_number>");
        puts("Type stream number (Ex:3) instead of <stream_number>, without angle brackets.");
        puts("Server will close stream number must be specified.");
    }
    else if (strcmp(video_file_one, "-1") == 0)
    {
        puts("Can not read channel one video file name from arguments make sure you use following configuration: ");
        puts("...    –ch1 <file_name>");
        puts("Type file name (Ex:'short_intro' or 'short_intro.txt') instead of <file_name>, without angle brackets.");
        puts("Server will close -ch1 video file must be specified.");
    }
    else
    {
        // If we are here mans server will work.
        // Still some arguments can be wrong which will get corrected anyways.
        if (strcmp(video_file_two, "-1") == 0)
        {
            puts("Optional channel two video file can not be read from arguments make sure you use following configuration: ");
            puts("...    –ch2 <file_name>");
            puts("Type file name (Ex:'short_intro' or 'short_intro.txt') instead of <file_name>, without angle brackets.");
            printf("You have selected %d streaming channels.", stream_number);
            puts("Program will continue to execute regardless.");
            puts("Channels without specified file name will stream first channels file.");
        }
        else if (strcmp(video_file_three, "-1") == 0)
        {
            puts("Optional channel three video file can not be read from arguments make sure you use following configuration: ");
            puts("...    –ch3 <file_name>");
            puts("Type file name (Ex:'short_intro' or 'short_intro.txt') instead of <file_name>, without angle brackets.");
            printf("You have selected %d streaming channels.", stream_number);
            puts("Program will continue to execute regardless.");
            puts("Channels without specified file name will stream first channels file.");
        }
        // We initialize server semaphores and memory.
        server_init(&argc, &argv);

        pid_t pid[MAX_PROC], rpid;         // Used to track child pids and kill them SIGUSR1 and wait them.
        struct sockaddr_in server, client; // Variables.
        int pidForkedCount = 0;
        ServerPID = getpid();           // Our server pid. Known by childs.
        signal(SIGUSR2, parent_signal); // Connecting SIGUSR2 to server, used for producer erroring.

        if ((pid[pidForkedCount] = fork()) == 0)
        { // fork a new process
            // Child
            // Will be used as a command line listener for server exit commands
            // This was not in document but my program listens to command line and exits safely.
            // This is suggested to use so no memory leak happens.
            // NO MEMORY LEAK CHECKED = 0 MEMORY LEAK.
            char str[100];
            while (server_status)
            {
                fgets(str, 100, stdin);
                // printf( "\nYou entered: \n");
                // puts( str );
                if (strncmp(str, "exit", 4) == 0)
                {
                    printf("Exit command received from server console.\n");
                    sem_wait(status_memaccess);
                    memcpy(shm_status_start, &zero, sizeof(int));
                    sem_post(status_memaccess);
                    printf("Server will terminate when next client tries to connect.\n");
                }
                else if (strncmp(str, "quit", 4) == 0)
                {
                    printf("Exit command received from server console.\n");
                    sem_wait(status_memaccess);
                    memcpy(shm_status_start, &zero, sizeof(int));
                    sem_post(status_memaccess);
                    printf("Server will terminate when next client tries to connect.\n");
                }
                else if (strncmp(str, "close", 5) == 0)
                {
                    printf("Exit command received from server console.\n");
                    sem_wait(status_memaccess);
                    memcpy(shm_status_start, &zero, sizeof(int));
                    sem_post(status_memaccess);
                    printf("Server will terminate when next client tries to connect.\n");
                }
                else if (strncmp(str, "server_close", 12) == 0)
                {
                    printf("Exit command received from server console.\n");
                    sem_wait(status_memaccess);
                    memcpy(shm_status_start, &zero, sizeof(int));
                    sem_post(status_memaccess);
                    printf("Server will terminate when next client tries to connect.\n");
                }
                else if (strncmp(str, "server_quit", 11) == 0)
                {
                    printf("Exit command received from server console.\n");
                    sem_wait(status_memaccess);
                    memcpy(shm_status_start, &zero, sizeof(int));
                    sem_post(status_memaccess);
                    printf("Server will terminate when next client tries to connect.\n");
                }
                else if (strncmp(str, "server_exit", 11) == 0)
                {
                    printf("Exit command received from server console.\n");
                    sem_wait(status_memaccess);
                    memcpy(shm_status_start, &zero, sizeof(int));
                    sem_post(status_memaccess);
                    printf("Server will terminate when next client tries to connect.\n");
                }
                // We set status memory.
                sem_wait(status_memaccess);
                memcpy(&server_status, (int *)shm_status_start, sizeof(int));
                sem_post(status_memaccess);
            }
            puts("Input listener child is closing.");
            child_close();
        }
        pidForkedCount++; // We increase every time we fork. Recording and tracking fork pids.

        int socket_desc, client_sock, c;

        // Create socket
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1)
        {
            perror("Could not create socket. Server will close due the Error!");

            server_close();
            return 1;
        }
        puts("Socket created");

        // Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(port);

        // Binding
        if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            // Print the error message
            perror("Binding failed. Server will close due the Error!");

            server_close();
            return 1;
        }
        printf("Binding done on port: %d \n", port);

        // Listen
        listen(socket_desc, 3);

        // Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);

        if (stream_number >= 1)
        {
            // Connecting SIGUSR1
            // We gonna fork producers and channels
            // They are terminated with SIGUSR1
            signal(SIGUSR1, child_close_signal);

            if ((pid[pidForkedCount] = fork()) == 0)
            { // fork a new process
                // Producer 1
                server_morph_producer(1, video_file_one, video_file_two, video_file_three);
            }
            pidForkedCount++;

            if (stream_number >= 2)
            {
                if ((pid[pidForkedCount] = fork()) == 0)
                { // fork a new process
                    // Producer 2
                    server_morph_producer(2, video_file_one, video_file_two, video_file_three);
                }
                pidForkedCount++;
            }
            if (stream_number >= 3)
            {
                if ((pid[pidForkedCount] = fork()) == 0)
                { // fork a new process
                    // Producer 3
                    server_morph_producer(3, video_file_one, video_file_two, video_file_three);
                }
                pidForkedCount++;
            }
            if (producer_one_error == 0)
            {
                if ((ChannelPIDs[0] = fork()) == 0)
                { // fork a new process
                    // Channel 1
                    server_morph_channel(1);
                }
                channel_one_init = 1;
            }

            if (producer_two_error == 0)
            {
                if (stream_number >= 2)
                {
                    if ((ChannelPIDs[1] = fork()) == 0)
                    { // fork a new process
                        // Channel 2
                        server_morph_channel(2);
                    }
                }
                channel_two_init = 1;
            }

            if (producer_three_error == 0)
            {
                if (stream_number >= 3)
                {
                    if ((ChannelPIDs[2] = fork()) == 0)
                    { // fork a new process
                        // Channel 3
                        server_morph_channel(3);
                    }
                }
                channel_three_init = 1;
            }

            // NOW WE ARE THE SERVER!
            // We only create worker threads from now on.
            // We constanly check server status memory space.
            // If input listener sets it to 0 we start closing server.
            while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)) && server_status)
            {
                char *message, client_message[2000];
                // This is a special case:
                // -We waiting for incoming client.
                // -Producer error happens.
                // -We go handle SIGUSR2
                // -We come back and skip 1 line
                // -Now we are here without actual client_sock.
                // if -1 we continue.
                if (client_sock == -1)
                {
                    continue;
                }

                // Check status memory, if we were about to close, we will not accept current client we will say sorry and close.
                sem_wait(status_memaccess);
                memcpy(&server_status, (int *)shm_status_start, sizeof(int));
                sem_post(status_memaccess);

                if (!server_status)
                {
                    // SAD but TRUE
                    message = "Sorry we were about to close!\n";
                    write(client_sock, message, strlen(message));
                    message = "Cya later!\n";
                    write(client_sock, message, strlen(message));
                    close(client_sock);
                    break;
                }

                // Connection accepted and we proceed.
                puts("Connection accepted");
                message = "Greetings! This is the streaming service.\n";
                write(client_sock, message, strlen(message));
                // We send this to client and it know everything is allright up until know.
                // And it sends us back which channel it wants to connect.
                recv(client_sock, client_message, 2000, 0);
                int worker_channel = atoi(client_message);
                // We will check if worker wants to connect valid channel or not.
                // Invalid channel is:
                // Producer gave error, couldn't read file etc.
                // Channel is closed too.
                if ((worker_channel == 1) && (channel_one_init == 0))
                {
                    // Checking if client wants to connect valid channel.
                    message = "Sorry this channel is not open right now!\n";
                    write(client_sock, message, strlen(message));
                    message = "Cya later!\n";
                    write(client_sock, message, strlen(message));
                    close(client_sock);
                    continue;
                }
                else if ((worker_channel == 2) && (channel_two_init == 0))
                {
                    message = "Sorry this channel is not open right now!\n";
                    write(client_sock, message, strlen(message));
                    message = "Cya later!\n";
                    write(client_sock, message, strlen(message));
                    close(client_sock);
                    continue;
                }
                else if ((worker_channel == 3) && (channel_three_init == 0))
                {
                    message = "Sorry this channel is not open right now!\n";
                    write(client_sock, message, strlen(message));
                    message = "Cya later!\n";
                    write(client_sock, message, strlen(message));
                    close(client_sock);
                    continue;
                }
                // If channel is here, we get a slot for it.
                int worker_slot = get_slot(worker_channel);

                if (worker_slot == -1)
                {
                    // If there is no slot for that channel.
                    message = "There is no available slot in this channel.\n";
                    write(client_sock, message, strlen(message));
                    close(client_sock);
                }
                else
                {
                    // We send this message so channel knows everything is going well.
                    message = "Please wait while we configure your connection.\n";
                    write(client_sock, message, strlen(message));
                    // Thread data is packaged.
                    struct thread_data td;
                    td.clientSock = client_sock;
                    td.channel = worker_channel;
                    td.workerSlot = worker_slot;
                    // And finaly worker thread is created.
                    if (pthread_create(&thread_id[current_thread_id], NULL, server_thread_worker, (void *)&td) < 0)
                    {
                        perror("Could not create thread");
                        return 1;
                    }
                    // Current thread id is increased we hold up to 9000 threads. Track them so we don't have memory leak at the end.
                    current_thread_id++;
                    puts("Handler assigned");
                }

                client_sock = -1;
            }

            if (client_sock < 0)
            {
                perror("Accept failed");
                return 1;
            }

            close(socket_desc);
            close(client_sock);
        }
        else
        {
            // If you put stream number.
            puts("Stream number must be bigger than 1.");
        }

        // Starting terminating children and threads.
        puts("Terminating children and thread.");
        // printf("%d \n",pid[0]);
        kill(pid[0], SIGINT); // Killing input listener child fork
        int child_status;
        // CHANNEL AND PRODUCERS HANDLE SIGUSR1 TO KILL THEMSELFS
        for (int pidNo = 1; pidNo < 4; pidNo++)
        {
            printf("Waiting for child pid: %d (producer %d)\n", pid[pidNo], pidNo);
            kill(pid[pidNo], SIGUSR1);
            rpid = waitpid(pid[pidNo], &child_status, WUNTRACED);
        }
        if (channel_one_init)
        {
            printf("Waiting for child pid: %d (channel %d)\n", ChannelPIDs[0], 1);
            kill(ChannelPIDs[0], SIGUSR1);
            rpid = waitpid(ChannelPIDs[0], &child_status, WUNTRACED);
        }
        if (channel_two_init)
        {
            printf("Waiting for child pid: %d (channel %d)\n", ChannelPIDs[1], 2);
            kill(ChannelPIDs[1], SIGUSR1);
            rpid = waitpid(ChannelPIDs[1], &child_status, WUNTRACED);
        }
        if (channel_three_init)
        {
        }
        printf("Waiting for child pid: %d (channel %d)\n", ChannelPIDs[2], 3);
        kill(ChannelPIDs[2], SIGUSR1);
        rpid = waitpid(ChannelPIDs[2], &child_status, WUNTRACED);

        for (int pidNo = 4; pidNo < pidForkedCount; pidNo++)
        {
            printf("Waiting for child pid: %d \n", pid[pidNo]);
            kill(pid[pidNo], SIGUSR1);
            rpid = waitpid(pid[pidNo], &child_status, WUNTRACED);
        }
        for (int threadNo = 0; threadNo < current_thread_id; threadNo++)
        {
            // Now join the thread , so that we dont terminate before the thread
            // WORKER THREADS
            pthread_kill(thread_id[threadNo], SIGUSR1);
            pthread_join(thread_id[threadNo], NULL);
        }
        puts("Terminated children and thread.");
    }

    // Finaly server closed.
    server_close();

    return 0;
}
