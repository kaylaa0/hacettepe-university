#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

int main(int argc, char *argv[])
{ // main function, we have got only 1 function in my code and it is main function.
    if (argc == 3)
    { // checking if our given argument number is valid
        struct flight_stur
        {                 // struct for flights
            char *name;   // flight name
            int id;       // flight number, good for identifying flights by their name
            int status;   // flight status, closed or open
            int business; // max business seat number
            int economy;  // max economy seat number
            int standard; // max standard seat number
            int bus_sold;
            int eco_sold;
            int standard_sold;
        };                                                                       // struct for flights
        struct flight_stur *f_catalog = calloc(100, sizeof(struct flight_stur)); // assigning memory for flights
        f_catalog[0].name = calloc(30, sizeof(char));                            // 0 number flight used to store total flights number
        strcpy(f_catalog[0].name, "Existing flights count");
        f_catalog[0].id = 0;
        struct passenger
        {                 // struct for passenger in flights
            char *name;   // passenger name
            char *want;   // which ticket passenger wanted to buy
            char *bought; // which ticket passenger bought
        };                // struct for passenger in flights
        struct passenger_stack
        { // struct for passenger STACKS! passengers are stored in stacks per flight.
            int top;
            struct passenger *passengers;
        };                                                                             // struct for passenger STACKS! passengers are stored in stacks per flight.
        struct passenger_stack *flights = calloc(100, sizeof(struct passenger_stack)); // assigning memory for flights, this is a collection of stacks
        struct queue_pas
        {                     // struct for passenger in queues
            char *name;       // passenger name
            char *want;       // which ticket passenger wants to buy
            int priority;     // passenger priority
            int old_priority; // this is to store priority in case if we want to initialize queue again, which happens after we can't sell tickets to some people, due non empty seats, we can put them back in queue and sell them again if seats become available, I think TA should have added that function.
        };                    // struct for passenger in queues
        struct queue_s
        {                                                            // struct for passenger PRIORITY QUEUES! queues are priority queues per flight.
            int front;                                               // front of queue
            int rear;                                                // rear of queue
            struct queue_pas *passengers;                            // data of queue
        };                                                           // struct for passenger PRIORITY QUEUES! queues are priority queues per flight.
        struct queue_s *queue = calloc(100, sizeof(struct queue_s)); // assigning memory to queue
        struct seat_stack
        {                                                                           // struct for empty seats STACK! Very unnecessary, work could have been done without it but TA wants this to exist so it is here.
            int top;                                                                // top of the stack
            int *data;                                                              // data of stack
        };                                                                          // struct for empty seats STACK! Very unnecessary, work could have been done without it but TA wants this to exist so it is here.
        struct seat_stack **empty_seats = calloc(100, sizeof(struct seat_stack *)); // assigning memory to empty seats stacks.
        int flight_number = 0;                                                      // this is unnecessary since we store existing flight number in f_catalog but, somewhere in my work i forgot i initialized it so i made this now it is too late to delete this, hope i don't get 0 because of this.
        FILE *commands;
        commands = fopen(argv[1], "r"); // reading input file
        if (commands == NULL)
        {
            printf("ERROR: Could not open commands file ");
        } // checking if input file is valid
        else
        {
            FILE *output;
            output = fopen(argv[2], "w"); // reading output file
            if (output == NULL)
            {
                printf("ERROR: Could not open output file ");
            } // checking if output file is valid
            else
            {
                char *line = calloc(100, sizeof(char));   // allocating line memory
                char *command = calloc(15, sizeof(char)); // allocating memory for readen command
                int i;                                    // int i, used in many places in the code, initialized here.
                while (fgets(line, 100 * sizeof(char), commands) != NULL)
                { // reading our file line by line
                    for (i = 0; line[i] != ' '; ++i)
                    {
                        command[i] = line[i];
                    } // Copying command from line
                    // BEGINNING OF ADDSEAT
                    if (!strcmp(command, "addseat"))
                    {
                        char *flight = calloc(15, sizeof(char)); // allocating memory for readen flight name
                        char *class = calloc(10, sizeof(char));  // allocating memory for readen class
                        char *count = calloc(5, sizeof(char));   // allocating memory for count
                        char *point = line;                      // getting a pointer for line, this is used to read the line, we will increase it by 1 and read the line char by char, down below is my function to do that, i used it in also the previous assignment.
                        int nchars, word = 0;
                        char *readen = calloc(15, sizeof(char)); // memory for readen word
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 15 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        flight[i] = readen[i];
                                    }
                                } // copying the words to memory
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        class[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        count[i] = readen[i];
                                    }
                                }
                                if (word == 6)
                                {
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        if ((!strcmp(class, "business") || !strcmp(class, "economy") || !strcmp(class, "standard")) && word <= 6 && strtol(count, &count + 4, 10) && strstr(count, ".") == NULL)
                        { // checking if given class is valid
                            int exist;
                            exist = 0;
                            if (f_catalog[0].id > 0)
                            {
                                for (i = 1; i <= f_catalog[0].id; i++)
                                {
                                    if (!strcmp(f_catalog[i].name, flight))
                                    {
                                        exist = 1;
                                    }
                                } // checking if the flight exist, if flight does not exist we are gonna assign memory for basic things like its name, status etc. if it exist we just reallocate memory of its size.
                            }
                            if (exist == 0)
                            {                                                                                        // if flight does not exist
                                f_catalog[0].id = f_catalog[0].id + 1;                                               // increase amount of flights that exist.
                                f_catalog[f_catalog[0].id].name = calloc(15, sizeof(char));                          // allocate memory for flights name
                                strcpy(f_catalog[f_catalog[0].id].name, flight);                                     // copy flights name
                                f_catalog[f_catalog[0].id].id = f_catalog[0].id;                                     // flights id is the amount of flights after it is added, for first flight we have 1 total number of flight after it is added so id is 1 etc.
                                f_catalog[f_catalog[0].id].status = 1;                                               // flights status set to open
                                flights[f_catalog[0].id].passengers = calloc(atoi(count), sizeof(struct passenger)); // allocating memory for passengers of flight
                                flights[f_catalog[0].id].top = -1;                                                   // top is set to -1 since there is nothing inside to stack yet
                                f_catalog[f_catalog[0].id].business = 0;                                             // max number of business seats in the flight in initialized.
                                f_catalog[f_catalog[0].id].economy = 0;                                              // max number of economy seats in the flight in initialized.
                                f_catalog[f_catalog[0].id].standard = 0;                                             // max number of standard seats in the flight in initialized.
                                empty_seats[f_catalog[0].id] = calloc(4, sizeof(struct seat_stack));                 // allocating memory for empty seats, empty seats are stored in 3 different stacks, business, economy and standard.
                                empty_seats[f_catalog[0].id][1].top = -1;                                            // initializing top of empty seat stacks
                                empty_seats[f_catalog[0].id][2].top = -1;                                            // initializing top of empty seat stacks
                                empty_seats[f_catalog[0].id][3].top = -1;                                            // initializing top of empty seat stacks
                                queue[f_catalog[0].id].passengers = calloc(400, sizeof(struct queue_pas));           // allocating memory for flight queue, it is 400 by default.
                                queue[f_catalog[0].id].front = -1;                                                   // initializing front value of queue.
                                queue[f_catalog[0].id].rear = -1;                                                    // initializing rear value of queue.
                                flight_number = flight_number + 1;                                                   // increasing existing number of flights by 1. we have 2 values to access this information as stated above.
                                if (!strcmp(class, "business"))
                                {                                                                            // if we are adding business seats
                                    f_catalog[f_catalog[0].id].business = atoi(count);                       // setting total business seats in catalog
                                    empty_seats[f_catalog[0].id][1].data = calloc(atoi(count), sizeof(int)); // allocating memory in empty seats
                                    empty_seats[f_catalog[0].id][1].top = atoi(count) - 1;                   // setting top of empty seats stack
                                }
                                if (!strcmp(class, "economy"))
                                { // basically same of above function done for economy
                                    f_catalog[f_catalog[0].id].economy = atoi(count);
                                    empty_seats[f_catalog[0].id][2].data = calloc(atoi(count), sizeof(int));
                                    empty_seats[f_catalog[0].id][2].top = atoi(count) - 1;
                                }
                                if (!strcmp(class, "standard"))
                                { // same done for standard
                                    f_catalog[f_catalog[0].id].standard = atoi(count);
                                    empty_seats[f_catalog[0].id][3].data = calloc(atoi(count), sizeof(int));
                                    empty_seats[f_catalog[0].id][3].top = atoi(count) - 1;
                                }
                                // printing the output
                                fprintf(output, "addseats %s %d %d %d\n", flight, f_catalog[f_catalog[0].id].business, f_catalog[f_catalog[0].id].economy, f_catalog[f_catalog[0].id].standard);
                            }
                            else if (exist == 1)
                            {
                                int flight_id;
                                flight_id = 0;
                                for (i = 1; i <= f_catalog[0].id; i++)
                                {
                                    if (!strcmp(f_catalog[i].name, flight))
                                    {
                                        flight_id = f_catalog[i].id;
                                    } // getting flight id from catalog, flight id is basically the number flight got initialized. First flight is 1 second is 2 etc.
                                }
                                if (f_catalog[flight_id].status == 1)
                                {                                                                                                                                                                                                                    // if flight exist and it is open, if it is closed we print error.
                                    flights[flight_id].passengers = realloc(flights[flight_id].passengers, (f_catalog[flight_id].business + f_catalog[flight_id].economy + f_catalog[flight_id].standard + atoi(count)) * sizeof(struct passenger)); // reallocating memory for flight passengers.
                                    if (!strcmp(class, "business"))
                                    { // setting values same as explained above.
                                        f_catalog[flight_id].business = f_catalog[flight_id].business + atoi(count);
                                        empty_seats[f_catalog[flight_id].id][1].data = realloc(empty_seats[f_catalog[flight_id].id][1].data, (empty_seats[f_catalog[flight_id].id][1].top + atoi(count)) * sizeof(int));
                                        empty_seats[f_catalog[flight_id].id][1].top = empty_seats[f_catalog[flight_id].id][1].top + atoi(count);
                                    }
                                    if (!strcmp(class, "economy"))
                                    {
                                        f_catalog[flight_id].economy = f_catalog[flight_id].economy + atoi(count);
                                        empty_seats[f_catalog[flight_id].id][2].data = realloc(empty_seats[f_catalog[flight_id].id][2].data, (empty_seats[f_catalog[flight_id].id][2].top + atoi(count)) * sizeof(int));
                                        empty_seats[f_catalog[flight_id].id][2].top = empty_seats[f_catalog[flight_id].id][2].top + atoi(count);
                                    }
                                    if (!strcmp(class, "standard"))
                                    {
                                        f_catalog[flight_id].standard = f_catalog[flight_id].standard + atoi(count);
                                        empty_seats[f_catalog[flight_id].id][3].data = realloc(empty_seats[f_catalog[flight_id].id][3].data, (empty_seats[f_catalog[flight_id].id][3].top + atoi(count)) * sizeof(int));
                                        empty_seats[f_catalog[flight_id].id][3].top = empty_seats[f_catalog[flight_id].id][3].top + atoi(count);
                                    }
                                    fprintf(output, "addseats %s %d %d %d\n", flight, f_catalog[flight_id].business, f_catalog[flight_id].economy, f_catalog[flight_id].standard); // output
                                }
                            }
                            else
                            {
                                fprintf(output, "error\n");
                            }
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(flight, 0, 15 * sizeof(char)); // freeing all used memory
                        free(flight);
                        memset(class, 0, 10 * sizeof(char));
                        free(class);
                        memset(count, 0, 5 * sizeof(char));
                        free(count);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNINGE OF ENQUEUE
                    else if (!strcmp(command, "enqueue"))
                    { // I don't explain everything line by line, it is just for first function, if you have seen my first assigment you must have an idea of my coding style, this work is done same, from now on i just point highlights
                        char *flight = calloc(15, sizeof(char));
                        char *class = calloc(10, sizeof(char));
                        char *pas_name = calloc(15, sizeof(char));
                        char *priority = calloc(10, sizeof(char));
                        int bus_que = 0;
                        priority[0] = '\0'; // priority set \0 by default, if it is not changed means priority is default, if it is changed it means we have either a veteran or diplomat.
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(15, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 15 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        flight[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        class[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        pas_name[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        priority[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        int flight_id;
                        flight_id = 0;
                        for (i = 1; i <= f_catalog[0].id; i++)
                        {
                            if (!strcmp(f_catalog[i].name, flight))
                            {
                                flight_id = f_catalog[i].id;
                            }
                        }
                        if (flight_id != 0 && f_catalog[flight_id].status == 1)
                        { // checking if flight exist if it does not we print error
                            if ((!strcmp(class, "standard") && priority[0] == '\0') ||
                                (!strcmp(class, "business") && priority[0] == '\0') || (!strcmp(class, "business") && !strcmp(priority, "diplomat")) || (!strcmp(class, "economy") && priority[0] == '\0') || (!strcmp(class, "economy") && !strcmp(priority, "veteran")))
                            {
                                if (queue[flight_id].front == -1 && queue[flight_id].rear == -1)
                                {                               // checking if the queue is empty
                                    queue[flight_id].front = 0; // initializing queue
                                    queue[flight_id].rear = 1;
                                    queue[flight_id].passengers[0].name = calloc(15, sizeof(char)); // allocating memory for passenger's name in queue
                                    strcpy(queue[flight_id].passengers[0].name, pas_name);          // copying passenger's name
                                    queue[flight_id].passengers[0].want = calloc(10, sizeof(char)); // allocating memory for passenger's wanted ticket
                                    strcpy(queue[flight_id].passengers[0].want, class);
                                    if (!strcmp(class, "business") && priority[0] != '\0')
                                    { // checking conditions 1 by 1 and checking if priority stated is different than default, if it is that means we have a diplomat or veteran.
                                        queue[flight_id].passengers[0].priority = 9;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 9 || queue[flight_id].passengers[i].priority == 8)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "business") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[0].priority = 8;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 9 || queue[flight_id].passengers[i].priority == 8)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "economy") && priority[0] != '\0')
                                    {
                                        queue[flight_id].passengers[0].priority = 7;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 7 || queue[flight_id].passengers[i].priority == 6)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "economy") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[0].priority = 6;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 7 || queue[flight_id].passengers[i].priority == 6)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "standard") && priority[0] != '\0')
                                    {
                                        queue[flight_id].passengers[0].priority = 5;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 5)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "standard") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[0].priority = 5;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 5)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                }
                                else if (queue[flight_id].front == queue[flight_id].rear)
                                {
                                    queue[flight_id].rear = queue[flight_id].rear + 1;
                                    queue[flight_id].passengers[queue[flight_id].rear - 1].name = calloc(15, sizeof(char));
                                    strcpy(queue[flight_id].passengers[queue[flight_id].rear - 1].name, pas_name);
                                    queue[flight_id].passengers[queue[flight_id].rear - 1].want = calloc(10, sizeof(char));
                                    strcpy(queue[flight_id].passengers[queue[flight_id].rear - 1].want, class);
                                    if (!strcmp(class, "business") && priority[0] != '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 9;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 9 || queue[flight_id].passengers[i].priority == 8)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "business") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 8;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 9 || queue[flight_id].passengers[i].priority == 8)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "economy") && priority[0] != '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 7;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 7 || queue[flight_id].passengers[i].priority == 6)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "economy") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 6;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 7 || queue[flight_id].passengers[i].priority == 6)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "standard") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 5;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 5)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    // printf("big error\n");//this should never happen since either they are -1 -1 or different than each other.
                                }
                                else
                                {
                                    queue[flight_id].rear = queue[flight_id].rear + 1;
                                    queue[flight_id].passengers[queue[flight_id].rear - 1].name = calloc(15, sizeof(char));
                                    strcpy(queue[flight_id].passengers[queue[flight_id].rear - 1].name, pas_name);
                                    queue[flight_id].passengers[queue[flight_id].rear - 1].want = calloc(10, sizeof(char));
                                    strcpy(queue[flight_id].passengers[queue[flight_id].rear - 1].want, class);
                                    if (!strcmp(class, "business") && priority[0] != '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 9;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 9 || queue[flight_id].passengers[i].priority == 8)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "business") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 8;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 9 || queue[flight_id].passengers[i].priority == 8)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "economy") && priority[0] != '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 7;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 7 || queue[flight_id].passengers[i].priority == 6)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "economy") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 6;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 7 || queue[flight_id].passengers[i].priority == 6)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                    else if (!strcmp(class, "standard") && priority[0] == '\0')
                                    {
                                        queue[flight_id].passengers[queue[flight_id].rear - 1].priority = 5;
                                        for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                        {
                                            if (queue[flight_id].passengers[i].priority == 5)
                                            {
                                                bus_que++;
                                            }
                                        }
                                        fprintf(output, "queue %s %s %s %d\n", flight, pas_name, class, bus_que);
                                    }
                                }
                            }
                            else
                            {
                                fprintf(output, "error\n");
                            }
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(flight, 0, 15 * sizeof(char));
                        free(flight);
                        memset(class, 0, 10 * sizeof(char));
                        free(class);
                        memset(pas_name, 0, 15 * sizeof(char));
                        free(pas_name);
                        memset(priority, 0, 10 * sizeof(char));
                        free(priority);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF SELL
                    else if (!strcmp(command, "sell"))
                    {
                        char *flight = calloc(15, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(15, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 15 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        flight[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        int flight_id;
                        flight_id = 0;
                        for (i = 1; i <= f_catalog[0].id; i++)
                        {
                            if (!strcmp(f_catalog[i].name, flight))
                            {
                                flight_id = f_catalog[i].id;
                            }
                        }
                        if (flight_id != 0 && f_catalog[flight_id].status == 1 && word <= 4)
                        {
                            int selling = 1;
                            int the_passenger;
                            while (selling == 1)
                            { // there is a loop here for selling all the available tickets, we break out of the loop if there are no passengers left in queue or no more passengers can be sold tickets.
                                the_passenger = queue[flight_id].front;
                                if (queue[flight_id].rear == -1 && queue[flight_id].front == -1)
                                {
                                    selling = 0; // breaking out immediately if queue is empty
                                }
                                else
                                {
                                    for (i = queue[flight_id].front + 1; i < queue[flight_id].rear; i++)
                                    {
                                        if (queue[flight_id].passengers[i].priority > queue[flight_id].passengers[the_passenger].priority)
                                        {
                                            the_passenger = i;
                                        }
                                    }
                                    if (queue[flight_id].passengers[the_passenger].priority == 0)
                                    {
                                        selling = 0;
                                    }
                                    else if (queue[flight_id].passengers[the_passenger].priority == 9 || queue[flight_id].passengers[the_passenger].priority == 8)
                                    {
                                        if (empty_seats[flight_id][1].top != -1)
                                        {
                                            empty_seats[flight_id][1].top = empty_seats[flight_id][1].top - 1;
                                            flights[flight_id].top = flights[flight_id].top + 1;
                                            flights[flight_id].passengers[flights[flight_id].top].want = calloc(10, sizeof(char));
                                            flights[flight_id].passengers[flights[flight_id].top].name = calloc(15, sizeof(char));
                                            flights[flight_id].passengers[flights[flight_id].top].bought = calloc(10, sizeof(char));
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].want, queue[flight_id].passengers[the_passenger].want);
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].name, queue[flight_id].passengers[the_passenger].name);
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].bought, "business");
                                            if (the_passenger == queue[flight_id].front)
                                            {
                                                queue[flight_id].front = queue[flight_id].front + 1;
                                            }
                                            else
                                            {
                                                for (i = the_passenger; i > queue[flight_id].front; i--)
                                                {
                                                    strcpy(queue[flight_id].passengers[i].name, queue[flight_id].passengers[i - 1].name);
                                                    strcpy(queue[flight_id].passengers[i].want, queue[flight_id].passengers[i - 1].want);
                                                    queue[flight_id].passengers[i].priority = queue[flight_id].passengers[i - 1].priority;
                                                    queue[flight_id].passengers[i].old_priority = queue[flight_id].passengers[i - 1].old_priority;
                                                }
                                                queue[flight_id].front = queue[flight_id].front + 1;
                                            }
                                            f_catalog[flight_id].bus_sold = f_catalog[flight_id].bus_sold + 1;
                                        }
                                        else if (empty_seats[flight_id][3].top != -1)
                                        {
                                            if (queue[flight_id].passengers[the_passenger].priority == 9)
                                            {
                                                queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                                queue[flight_id].passengers[the_passenger].priority = 4;
                                            }
                                            else
                                            {
                                                queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                                queue[flight_id].passengers[the_passenger].priority = 3;
                                            }
                                        }
                                        else
                                        {
                                            queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                            queue[flight_id].passengers[the_passenger].priority = 0;
                                        }
                                    }
                                    else if (queue[flight_id].passengers[the_passenger].priority == 7 || queue[flight_id].passengers[the_passenger].priority == 6)
                                    {
                                        if (empty_seats[flight_id][2].top != -1)
                                        {
                                            empty_seats[flight_id][2].top = empty_seats[flight_id][2].top - 1;
                                            flights[flight_id].top = flights[flight_id].top + 1;
                                            flights[flight_id].passengers[flights[flight_id].top].want = calloc(10, sizeof(char));
                                            flights[flight_id].passengers[flights[flight_id].top].name = calloc(15, sizeof(char));
                                            flights[flight_id].passengers[flights[flight_id].top].bought = calloc(10, sizeof(char));
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].want, queue[flight_id].passengers[the_passenger].want);
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].name, queue[flight_id].passengers[the_passenger].name);
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].bought, "economy");
                                            if (the_passenger == queue[flight_id].front)
                                            {
                                                queue[flight_id].front = queue[flight_id].front + 1;
                                            }
                                            else
                                            {
                                                for (i = the_passenger; i > queue[flight_id].front; i--)
                                                {
                                                    strcpy(queue[flight_id].passengers[i].name, queue[flight_id].passengers[i - 1].name);
                                                    strcpy(queue[flight_id].passengers[i].want, queue[flight_id].passengers[i - 1].want);
                                                    queue[flight_id].passengers[i].priority = queue[flight_id].passengers[i - 1].priority;
                                                    queue[flight_id].passengers[i].old_priority = queue[flight_id].passengers[i - 1].old_priority;
                                                }
                                                queue[flight_id].front = queue[flight_id].front + 1;
                                            }
                                            f_catalog[flight_id].eco_sold = f_catalog[flight_id].eco_sold + 1;
                                        }
                                        else if (empty_seats[flight_id][3].top != -1)
                                        {
                                            if (queue[flight_id].passengers[the_passenger].priority == 7)
                                            {
                                                queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                                queue[flight_id].passengers[the_passenger].priority = 2;
                                            }
                                            else
                                            {
                                                queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                                queue[flight_id].passengers[the_passenger].priority = 1;
                                            }
                                        }
                                        else
                                        {
                                            queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                            queue[flight_id].passengers[the_passenger].priority = 0;
                                        }
                                    }
                                    else if (queue[flight_id].passengers[the_passenger].priority == 5 || queue[flight_id].passengers[the_passenger].priority == 4 || queue[flight_id].passengers[the_passenger].priority == 3 || queue[flight_id].passengers[the_passenger].priority == 2 || queue[flight_id].passengers[the_passenger].priority == 1)
                                    {
                                        if (empty_seats[flight_id][3].top != -1)
                                        {
                                            empty_seats[flight_id][3].top = empty_seats[flight_id][3].top - 1;
                                            flights[flight_id].top = flights[flight_id].top + 1;
                                            flights[flight_id].passengers[flights[flight_id].top].want = calloc(10, sizeof(char));
                                            flights[flight_id].passengers[flights[flight_id].top].name = calloc(15, sizeof(char));
                                            flights[flight_id].passengers[flights[flight_id].top].bought = calloc(10, sizeof(char));
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].want, queue[flight_id].passengers[the_passenger].want);
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].name, queue[flight_id].passengers[the_passenger].name);
                                            strcpy(flights[flight_id].passengers[flights[flight_id].top].bought, "standard");
                                            if (the_passenger == queue[flight_id].front)
                                            {
                                                queue[flight_id].front = queue[flight_id].front + 1;
                                            }
                                            else
                                            {
                                                for (i = the_passenger; i > queue[flight_id].front; i--)
                                                {
                                                    strcpy(queue[flight_id].passengers[i].name, queue[flight_id].passengers[i - 1].name);
                                                    strcpy(queue[flight_id].passengers[i].want, queue[flight_id].passengers[i - 1].want);
                                                    queue[flight_id].passengers[i].priority = queue[flight_id].passengers[i - 1].priority;
                                                    queue[flight_id].passengers[i].old_priority = queue[flight_id].passengers[i - 1].old_priority;
                                                }
                                                queue[flight_id].front = queue[flight_id].front + 1;
                                            }
                                            f_catalog[flight_id].standard_sold = f_catalog[flight_id].standard_sold + 1;
                                        }
                                        else
                                        {
                                            if (queue[flight_id].passengers[the_passenger].old_priority == 6 || queue[flight_id].passengers[the_passenger].old_priority == 7 || queue[flight_id].passengers[the_passenger].old_priority == 8 || queue[flight_id].passengers[the_passenger].old_priority == 9)
                                            {
                                                queue[flight_id].passengers[the_passenger].priority = 0;
                                            }
                                            else
                                            {
                                                queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                                queue[flight_id].passengers[the_passenger].priority = 0;
                                            }
                                        }
                                    }
                                }
                            }
                            fprintf(output, "sold %s %d %d %d\n", flight, f_catalog[flight_id].bus_sold, f_catalog[flight_id].eco_sold, f_catalog[flight_id].standard_sold);
                            for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                            {
                                queue[flight_id].passengers[i].priority = queue[flight_id].passengers[i].old_priority;
                                queue[flight_id].passengers[i].old_priority = 0;
                            }
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(flight, 0, 15 * sizeof(char));
                        free(flight);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF CLOSE
                    else if (!strcmp(command, "close"))
                    {
                        char *flight = calloc(15, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(15, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 15 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        flight[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        int flight_id;
                        flight_id = 0;
                        for (i = 1; i <= f_catalog[0].id; i++)
                        {
                            if (!strcmp(f_catalog[i].name, flight))
                            {
                                flight_id = f_catalog[i].id;
                            }
                        }
                        if (flight_id != 0 && f_catalog[flight_id].status == 1 && word <= 4)
                        {
                            f_catalog[flight_id].status = 0; // setting flight status to closed
                            int printing = 1;
                            if (queue[flight_id].rear - queue[flight_id].front == -2)
                            { // printing output
                                fprintf(output, "closed %s %d 0\n", flight, flights[flight_id].top + 1);
                                printing = 0;
                            }
                            else
                            {
                                fprintf(output, "closed %s %d %d\n", flight, flights[flight_id].top + 1, queue[flight_id].rear - queue[flight_id].front);
                            }
                            int the_passenger;
                            the_passenger = queue[flight_id].front;
                            while (printing == 1)
                            {
                                for (i = queue[flight_id].front; i < queue[flight_id].rear; i++)
                                {
                                    if (queue[flight_id].passengers[i].priority > queue[flight_id].passengers[the_passenger].priority)
                                    {
                                        the_passenger = i;
                                    }
                                }
                                if (queue[flight_id].passengers[the_passenger].priority == 0)
                                {
                                    printing = 0;
                                }
                                else
                                {
                                    fprintf(output, "waiting %s\n", queue[flight_id].passengers[the_passenger].name);
                                    queue[flight_id].passengers[the_passenger].old_priority = queue[flight_id].passengers[the_passenger].priority;
                                    queue[flight_id].passengers[the_passenger].priority = 0;
                                }
                            }
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(flight, 0, 15 * sizeof(char));
                        free(flight);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF REPORT
                    else if (!strcmp(command, "report"))
                    {
                        char *flight = calloc(15, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(15, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 15 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        flight[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        int flight_id;
                        flight_id = 0;
                        for (i = 1; i <= f_catalog[0].id; i++)
                        {
                            if (!strcmp(f_catalog[i].name, flight))
                            {
                                flight_id = f_catalog[i].id;
                            }
                        }
                        if (flight_id != 0 && word <= 4)
                        {
                            fprintf(output, "report %s\n", flight);
                            int bus_tic = 0;
                            int eco_tic = 0;
                            int sta_tic = 0;
                            for (i = 0; i <= flights[flight_id].top; i++)
                            {
                                if (!strcmp(flights[flight_id].passengers[i].bought, "business"))
                                { // counting total tickets of classes and storing them
                                    bus_tic++;
                                }
                                else if (!strcmp(flights[flight_id].passengers[i].bought, "economy"))
                                {
                                    eco_tic++;
                                }
                                else if (!strcmp(flights[flight_id].passengers[i].bought, "standard"))
                                {
                                    sta_tic++;
                                }
                            }
                            fprintf(output, "business %d\n", bus_tic); // printing
                            for (i = 0; i <= flights[flight_id].top; i++)
                            {
                                if (!strcmp(flights[flight_id].passengers[i].bought, "business"))
                                {
                                    fprintf(output, "%s\n", flights[flight_id].passengers[i].name);
                                }
                            }
                            fprintf(output, "economy %d\n", eco_tic);
                            for (i = 0; i <= flights[flight_id].top; i++)
                            {
                                if (!strcmp(flights[flight_id].passengers[i].bought, "economy"))
                                {
                                    fprintf(output, "%s\n", flights[flight_id].passengers[i].name);
                                }
                            }
                            fprintf(output, "standard %d\n", sta_tic);
                            for (i = 0; i <= flights[flight_id].top; i++)
                            {
                                if (!strcmp(flights[flight_id].passengers[i].bought, "standard"))
                                {
                                    fprintf(output, "%s\n", flights[flight_id].passengers[i].name);
                                }
                            }
                            fprintf(output, "end of report %s\n", flight);
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(flight, 0, 15 * sizeof(char));
                        free(flight);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF INFO
                    else if (!strcmp(command, "info"))
                    {
                        char *passenger = calloc(15, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(15, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 15 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        passenger[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        int j;
                        int found = 0;
                        for (i = 1; i <= flight_number; i++)
                        { // checking flight passengers for the passenger
                            for (j = 0; j <= flights[i].top; j++)
                            {
                                if (!strcmp(flights[i].passengers[j].name, passenger))
                                {
                                    found = 1;
                                    fprintf(output, "info %s %s %s %s\n", passenger, f_catalog[i].name, flights[i].passengers[j].want, flights[i].passengers[j].bought);
                                }
                            }
                        }
                        for (i = 1; i <= flight_number; i++)
                        { // checking queue for passenger, if they are not sold ticket yet or couldn't find seat, they are still in queue, may be a seat will be available.
                            for (j = queue[i].front; j < queue[i].rear; j++)
                            {
                                if (!strcmp(queue[i].passengers[j].name, passenger))
                                {
                                    found = 1;
                                    fprintf(output, "info %s %s %s none\n", passenger, f_catalog[i].name, queue[i].passengers[j].want);
                                }
                            }
                        }
                        if (found == 0)
                        {
                            fprintf(output, "error\n");
                        }
                        memset(passenger, 0, 15 * sizeof(char));
                        free(passenger);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // Printing error if command is invalid, meaning also line is invalid.
                    else if (*command != 13)
                    {
                        fprintf(output, "error\n");
                    }
                    memset(command, 0, 15 * sizeof(char));
                }
                memset(command, 0, 15 * sizeof(char));
                free(command);
                memset(line, 0, 100 * sizeof(char));
                free(line);
            }
            fclose(output); // closing the output file, now the work is done hopefully there is no output errors
        }
        fclose(commands); // closing the input file.
        int i, j;         // BEGINNING OF MEMORY FREEING OF DATABASE
        for (i = 1; i <= flight_number; i++)
        { // Check my report paper for visualized view of my data base to understand it better.
            for (j = 0; j <= 3; j++)
            { // freeing empty seats stacks
                free(empty_seats[i][j].data);
            }
            free(empty_seats[i]);
        }
        free(empty_seats);
        for (i = 0; i <= flight_number; i++)
        { // freeing catalog
            free(f_catalog[i].name);
        }
        free(f_catalog);
        for (i = 1; i <= flight_number; i++)
        {
            for (j = 0; j <= flights[i].top; j++)
            { // free flights stacks
                free(flights[i].passengers[j].name);
                free(flights[i].passengers[j].want);
                free(flights[i].passengers[j].bought);
            }
            free(flights[i].passengers);
        }
        free(flights);
        for (i = 1; i <= flight_number; i++)
        {
            for (j = 0; j < queue[i].rear; j++)
            { // freeing queue PRIORITY queues
                free(queue[i].passengers[j].want);
                free(queue[i].passengers[j].name);
            }
            free(queue[i].passengers);
        }
        free(queue);
    }
    else if (argc > 3)
    {
        printf("ERROR: Too many arguments supplied.\n");
    }
    else
    {
        printf("ERROR: Insufficent arguments.\n");
    }
    return 1; // ending the main, thanks if you read the comments :)
}