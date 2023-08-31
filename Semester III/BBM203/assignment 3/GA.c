#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GA.h"

/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

int main(int argc, char *argv[])
{ // main function
    if (argc == 4)
    {                                          // checking if our given argument number is valid
        int i, j;                              // main integers used everywhere initialized here
        struct chromosome *chromo;             // our main linked list data base, this chromo stores best chromo, and points to first chromo of current population
        FILE *population;                      // defining population file
        population = fopen("population", "r"); // reading population file
        if (population == NULL)
        {
            printf("ERROR: Could not read population file ");
        } // checking if population file is valid
        else
        {
            char *line = calloc((atoi(argv[1]) * 2) + 2, sizeof(char));               // our main line string for reading population file
            struct chromosome *tempchromosome = calloc(1, sizeof(struct chromosome)); // a temp chromosome to initialize our first chromo, the best chromo
            tempchromosome->genes = NULL;                                             // setting genes to NULL means it is not initialized
            tempchromosome->next = NULL;                                              // next pointer is NULL means this is the last item in linked list
            tempchromosome->rank = 0;                                                 // rank of best chromo is always 0
            chromo = tempchromosome;                                                  // linking our temp chromo
            while (fgets(line, (atoi(argv[1]) * 2) + 2 * sizeof(char), population) != NULL)
            {                                          // reading population file
                char *point = line;                    // this is my reading function, I prefer reading char by char and break at point if given char meets requirements, than we scan the word
                int valno = 0;                         // this is used to foresee ending of the file and freeing junk memory allocated for temp chromo
                char *value = calloc(3, sizeof(char)); // readen value
                valno++;
                struct chromosome *tempchromosome2 = calloc(1, sizeof(struct chromosome)); // a temp chromosome
                tempchromosome2->genes = NULL;                                             // setting temp chromosome
                tempchromosome2->next = NULL;
                tempchromosome2->rank = 0;
                struct genes *tempgene = calloc(1, sizeof(struct genes)); // temp genes linked list
                tempgene->val = 0;                                        // this is fitness score, every 0 element, head, of the linked list genes, is fitness score, currently set to 0, in to future when we calculate will be written over
                tempgene->next = NULL;                                    // NULL means this is the last item of the linked list, indeed it is since it is the first item
                tempchromosome2->genes = tempgene;                        // pointing temp chromosome to tempgene
                while (*point)
                {                                                              // starting reading function
                    struct genes *tempgene2 = calloc(1, sizeof(struct genes)); // tempgene2 will be initialized while tempgene will be just a pointer to our current position in tempchromosome2
                    tempgene2->val = 0;
                    tempgene2->next = NULL;
                    memset(value, 0, 3 * sizeof(char));
                    for (; *point; point++)
                    {
                        if ((*point >= '0' && *point <= '9'))
                        {
                            break;
                        }
                    }
                    if (sscanf(point, "%c", value) == 1)
                    {
                        tempgene2->val = atoi(value); // assigning value to tempgene2
                        tempgene->next = tempgene2;   // linking tempgene2 to our current position in linked list
                        tempgene = tempgene->next;    // moving our current position to last link in linked list
                    }
                    if (valno == (atoi(argv[1])) + 1)
                    { // this is a bug fix, freeing tempgene2 before we are out of loop and detecting end of the file beforehand
                        free(tempgene2);
                    }
                    point++; // next char
                    valno++; // next value
                }
                tempchromosome->next = tempchromosome2; // linking tempchromosome2 to our linked list
                tempchromosome = tempchromosome->next;  // moving to next chromosome, aka next line
                free(value);                            // some memory freeing done here
            }
            fclose(population); // closing the population file.
            FILE *xover;        // reading cover file
            xover = fopen("xover", "r");
            FILE *selection; // reading selection file
            selection = fopen("selection", "r");
            FILE *mutatef; // reading mutate file, we have function named mutate so this has to be called mutatef, f stands for file.
            mutatef = fopen("mutate", "r");
            char *line1 = calloc(20, sizeof(char));
            char *line2 = calloc(((atoi(argv[2])) * 3) + 2, sizeof(char)); // he are some dynamic lines to read our files
            char *line3 = calloc(10, sizeof(char));
            int j = 0; // generation counter is set to 0

            printf("GENERATION: %d\n", j);  // printing for generation 0
            calcfitness(chromo->next);      // calculating fitness
            sortall(chromo, atoi(argv[2])); // sorting linked list
            findgold(chromo, chromo->next); // finding best chromo and storing it
            calcranks(chromo);              // calcualting ranks, there is no output of this function but it does what is should do, calculating and storing ranked values
            printchromos(chromo);           // printing

            while ((fgets(line3, 10 * sizeof(char), mutatef) != NULL) && (fgets(line1, 20 * sizeof(char), xover) != NULL) && (fgets(line2, (((atoi(argv[2])) * 3) + 2) * sizeof(char), selection) != NULL))
            {                        // reading 3 files at the same time line by line,
                j++;                 // moving to next generation
                char *point = line2; // again my read function as explained above
                int nchars = 0;
                char *readen = calloc(10, sizeof(char)); // memory for readen word, now for each line readen in all of the files, at total MAX_GEN, we have POP_SIZE/2  different values in selection file, for each value pair we read as a word to this string and later on call our crossover function for each value pair.
                while (*point)
                {
                    memset(readen, 0, 10 * sizeof(char)); // resetting readen memory for our loop
                    for (; *point; point++)
                    {
                        if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                        {
                            break;
                        }
                    }
                    if (sscanf(point, "%s%n", readen, &nchars) == 1)
                    {                                                      // found first pair
                        int abc, xs, xe, c1, c2;                           // our integers which will go in to crossover function, xs = start, xe = end, c1 = chromosome 1, c2 = chromosome 2
                        xe = atoi(strstr(line1, ":") + 1 * sizeof(char));  // ending is line1's after ":"
                        c2 = atoi(strstr(readen, ":") + 1 * sizeof(char)); // chromosome 2 is readen pair's after ":"
                        char *temp = calloc(10, sizeof(char));             // a temp string so we read before ":" in to this and get integer values
                        for (abc = 0; abc < 10; abc++)
                        {
                            if (line1[abc] == ':')
                            { // reading line1 until ":"
                                break;
                            }
                            else
                            {
                                temp[abc] = line1[abc]; // copying string
                            }
                        }
                        xs = atoi(temp); // storing value to according integer
                        for (abc = 0; abc < 10; abc++)
                        {
                            temp[abc] = '\0';
                        }
                        for (abc = 0; abc < 10; abc++)
                        {
                            if (readen[abc] == ':')
                            { // reading readen until ":"
                                break;
                            }
                            else
                            {
                                temp[abc] = readen[abc]; // copying string
                            }
                        }
                        c1 = atoi(temp);                    // chromosome 1 is readen pair's before ":"
                        crossover(chromo, xs, xe, c1, c2);  // calling our crossover function per readen pair
                        memset(temp, 0, 10 * sizeof(char)); // setting our used temp memory to 0
                        free(temp);                         // freeing temp memory
                    }
                    point += nchars; // moving nchar, readen amount of chars, forward in our reading pointer
                }
                // printf("%s%s%s",line1,line2,line3); this is a developer function
                mutate(chromo->next, atoi(line3));    // calling mutate function
                printf("GENERATION: %d\n", j);        // printing generation number
                calcfitness(chromo->next);            // calculating fitness
                sortall(chromo, atoi(argv[2]));       // sorting linked list
                findgold(chromo, chromo->next);       // finding and storing best value
                calcranks(chromo);                    // calculating ranks and storing them
                printchromos(chromo);                 // printing chromosome linked list database
                memset(readen, 0, 10 * sizeof(char)); // setting readen memory to 0
                free(readen);                         // freeing readen memory
            }
            fclose(xover); // closing our files
            fclose(selection);
            fclose(mutatef);
            memset(line1, 0, (20 * sizeof(char))); // setting our readen line memories to 0 and freeing them
            free(line1);
            memset(line2, 0, (((atoi(argv[2])) * 3) + 2) * sizeof(char));
            free(line2);
            memset(line3, 0, (10 * sizeof(char)));
            free(line3);
            memset(line, 0, ((atoi(argv[1]) * 2) + 2) * sizeof(char));
            free(line);
        }
        // BEGINNING OF MEMORY FREEING OF DATABASE
        freechromem(chromo); // calling memory freeing function
    }
    else if (argc > 4)
    {
        printf("ERROR: Too many arguments supplied.\n");
    } // error for too many arguments
    else
    {
        printf("ERROR: Insufficent arguments.\n");
    }         // error for not enough arguments
    return 1; // ending the main, :)
}
void calcranks(chromosome *chromo)
{
    putranks(chromo->next, getworst(chromo->next, chromo->next->next)->genes->val, getbest(chromo->next, chromo->next->next)->genes->val);
} // rank calculator function takes our main chromo database as input. This function uses putranks, getworst and getbest function and combines them to 1 line.
void putranks(chromosome *data, int worst, int best)
{
    if (data->next == NULL)
    { // checking if we are at last link, if we are we are not gonna continue recursion
        if (worst == best)
        { // if worst == best dividing by 0 is invalid so we assign all ranks to 0 according to assignment paper
            data->rank = 0;
        }
        else
        {
            data->rank = ((float)data->genes->val - (float)best) / ((float)worst - (float)best); // rank calculator function
        }
    }
    else
    {
        if (worst == best)
        {
            data->rank = 0;
            putranks(data->next, worst, best);
        }
        else
        {
            data->rank = ((float)data->genes->val - (float)best) / ((float)worst - (float)best);
            putranks(data->next, worst, best); // moving in to recursion steps until we reach last link
        }
    }
} // this function calculates rank values and stores them
chromosome *getbest(chromosome *current, chromosome *comp)
{
    if (comp->next == NULL)
    { // we check given first 2 values and move in to recursion with best of 2 and next value, until we reach last link, and at the last link we check one last time and return
        if (current->genes->val < comp->genes->val)
        {
            return current;
        }
        else
        {
            return comp;
        }
    }
    else
    {
        if (current->genes->val < comp->genes->val)
        {
            getbest(current, comp->next);
        }
        else
        {
            getbest(comp, comp->next);
        }
    }
} // function for getting best chromosome
chromosome *getworst(chromosome *current, chromosome *comp)
{
    if (comp->next == NULL)
    { // we check given first 2 values and move in to recursion with worst of 2 and next value, until we reach last link, and at the last link we check one last time and return
        if (current->genes->val > comp->genes->val)
        {
            return current;
        }
        else
        {
            return comp;
        }
    }
    else
    {
        if (current->genes->val > comp->genes->val)
        {
            getworst(current, comp->next);
        }
        else
        {
            getworst(comp, comp->next);
        }
    }
} // function for getting worst chromosome
void findgold(chromosome *chromo, chromosome *compare)
{
    if (compare == NULL)
    { // if we are at the end of the linked list return
        return;
    }
    if (chromo->genes == NULL)
    { // if this is the first time we are finding the most fit chromosome, it won't be initialized so we have different function for that, we copy first chromosome as our best and move to next recursion step
        initfrom(chromo, chromo->next);
        findgold(chromo, chromo->next);
    }
    else
    {
        if (compare->genes->val < chromo->genes->val)
        {
            copychromo(chromo, compare); // if the compared chromosome is better copying that chromosome as our most fit chromosome
            findgold(chromo, compare->next);
        }
        else
        {
            findgold(chromo, compare->next); // this is a possible bug fix
        }
    }
} // this function finds and stores most fit chromosome
void initfrom(chromosome *c1, chromosome *c2)
{
    genes *tempgenex = calloc(1, sizeof(struct genes)); // no need to free this since it is linked to main database and will be freed at the end of main function
    genes *readgenex;
    tempgenex->val = c2->genes->val;
    tempgenex->next = NULL;
    c1->genes = tempgenex;
    readgenex = c2->genes->next;
    while (readgenex != NULL)
    { // reading our second given chromosome gene by gene and passing to first one
        struct genes *tempgenex2 = calloc(1, sizeof(struct genes));
        tempgenex2->val = readgenex->val;
        tempgenex2->next = NULL;
        tempgenex->next = tempgenex2;
        readgenex = readgenex->next;
        tempgenex = tempgenex->next;
    }
} // this function initializes a chromosome and copies other given chromosome in to that one.
void copychromo(chromosome *c1, chromosome *c2)
{
    genes *readgenex1 = c1->genes;
    genes *readgenex2 = c2->genes;
    while (readgenex1 != NULL)
    {
        readgenex1->val = readgenex2->val;
        readgenex1 = readgenex1->next;
        readgenex2 = readgenex2->next;
    }
} // copying a chromosome to another chromosome, c2 to c1
void copyadet(genes *c1, genes *c2, int a)
{
    genes *readgenex1 = c1;
    genes *readgenex2 = c2;
    int i;
    for (i = a; i > 0; i--)
    {
        // printf("copying %d from %d \n",readgenex1->val,readgenex2->val); a developer function
        readgenex1->val = readgenex2->val;
        readgenex1 = readgenex1->next;
        readgenex2 = readgenex2->next;
    }
} // copying starting from given gene point, a given int a amount. Used in crossover function.
void crossover(chromosome *chromo, int xs, int xe, int c1, int c2)
{
    chromosome *temp = calloc(1, sizeof(chromosome));       // allocating memory for temp chromosome
    initfrom(temp, getme(chromo, c2));                      // copying c2 to a temp chromosome
    copypart(getme(chromo, c2), getme(chromo, c1), xs, xe); // copying wanted part of c1 to c2
    copypart(getme(chromo, c1), temp, xs, xe);              // copying wanted part of c2 to c1 using temp
    freechromem(temp);                                      // freeing temp chromosome since it is not linked to main database we have to free it here
} // main crossover function, we first allocate a temp chromosome and copy contents of c2 than copy the wanted part of c1 to c2 and copy wanted part of c2 to c1 by using our temp chromosome which we previously copied contents of c2 to.
chromosome *getme(chromosome *chromo, int x)
{
    if (x == 0)
    { // if we reached 0 we reached our wanted chromosome, returning. If function is called with 0 it will return most fit chromosome
        return chromo;
    }
    else
    {
        getme(chromo->next, x - 1); // decreasing x by 1 and moving to next chromosome
    }
} // taken main database chromo and an integer x as function paramaters, return xth chromosome in the population
genes *getmegene(genes *start, int x)
{
    if (x == 0)
    { // if x is 0 means we reached to our wanted position returning gene
        return start;
    }
    else
    {
        getmegene(start->next, x - 1); // decreasing x by 1 and moving to next link in linked list
    }
} // taken a starting gene, moves x amount in the linked list
void copypart(chromosome *c1, chromosome *c2, int xs, int xe)
{
    copyadet(getmegene(c1->genes, xs), getmegene(c2->genes, xs), xe - xs + 1);
} // copying wanted part of c2 to c1, this function uses getmegene and copyadet function and puts them together in 1 line
void mutate(chromosome *data, int pos)
{
    if (data->next == NULL)
    {                                                 // break from recursion if we are at the last chromosome
        genes *thegene = getmegene(data->genes, pos); // get the wanted gene
        if (thegene->val == 0)
        {
            thegene->val = 1; // reverse its value
        }
        else if (thegene->val == 1)
        {
            thegene->val = 0;
        }
    }
    else
    {
        genes *thegene = getmegene(data->genes, pos);
        if (thegene->val == 0)
        {
            thegene->val = 1;
        }
        else if (thegene->val == 1)
        {
            thegene->val = 0;
        }
        mutate(data->next, pos); // move to next chromosome
    }
} // the mutate function, we start with first chromosome in the population, using getmegene function we get the wanted gene and modify its value.
void sortall(chromosome *chromo, int argv)
{
    int i;
    for (i = 0; i < ((argv * argv) / 2) + 1; i++)
    { // running sortgenes function enough times switch sort
        // printf("\n%d\n",i);
        while (sortgenes(chromo->next))
        { // running sortgenes function until it returns 0
            // printall(chromo->next);
        }
        if (chromo->next->genes->val > chromo->next->next->genes->val)
        { // checking if first 2 chromosome should be switched
            chromosome *temp = chromo->next->next->next;
            chromosome *temp2 = chromo->next->next;
            chromo->next->next->next = chromo->next;
            chromo->next->next = temp;
            chromo->next = temp2;
        }
    }
} // sorting function argv 2 and main database as input, calls sortgenes function and also checks if the first 2 chromosome should be switched, sorting is where my project loses most of runtime. not very efficent
int sortgenes(chromosome *data)
{
    if (data->next->next == NULL)
    { // if we are at the second from last link, we check if last 2 are sorted, if not we run the function 1 more time.
        if (data->genes->val > data->next->genes->val)
        {
            return 1;
        }
        else
        {
            return 0; // returning 0 means we sorted successfully
        }
    }
    else
    {
        if (data->next->genes->val > data->next->next->genes->val)
        { // switching pairs and returning 1
            chromosome *temp = data->next->next->next;
            chromosome *temp2 = data->next->next;
            data->next->next->next = data->next;
            data->next->next = temp;
            data->next = temp2;
            return 1;
        }
        else
        {
            sortgenes(data->next); // if our 2 pairs are sorted we read next pairs
        }
    }
} // taken chromosome database as paramater, checks second and third values of population and switches them if they should be, any switch to first value is done outside of this function
void calcfitness(chromosome *data)
{
    if (data->next == NULL)
    {                                            // checking if we are at the last chromosome
        int total = 0;                           // initializing total fitness score to 0
        givemenumber(&total, data->genes->next); // give me number calculates the genes as if they are binary
        data->genes->val = total;                // storing the total
    }
    else
    {
        int total = 0;
        givemenumber(&total, data->genes->next);
        data->genes->val = total;
        calcfitness(data->next); // moving next chromosome
    }
} // calculates fitness scores and stores them in the first gene of chromosomes
int givemenumber(int *total, genes *start)
{
    if (start->next == NULL)
    {                         // if we are at last gene, also means first bit
        *total += start->val; // we add bit to total, if it is 1 we add 1 if it is 0 we add 0
        return 2;             // return 2 to previous step of recursion, this will used to multiply previous,second , bit by 2
    }
    else
    {
        int x = givemenumber(total, start->next); // going in to recursion and getting 2's compliment of the bit
        *total += start->val * x;                 // multiplying our current bit value with 2's compliment bit value, if 0 it will add 0 if 1 it will add 2's compliment current position bit value
        return x * 2;                             // multiplying our current bit value by 2 and returning it to previous step of the recursion
    }
} // calculating genes as if they are binary and storing value to total
void printchromos(chromosome *chromo)
{
    printall(chromo->next);
    printf("Best chromosome found so far: ");
    printgene(chromo->genes->next);
    printf(" -> %d\n", chromo->genes->val);
} // main print function, first prints all chromosomes and prints best fitness according to assignment paper outputs
void printall(chromosome *data)
{
    if (data->next == NULL)
    {
        printgene(data->genes->next);
        printf(" -> %d\n", data->genes->val);
        return;
    }
    else
    {
        printgene(data->genes->next);
        printf(" -> %d\n", data->genes->val);
        printall(data->next);
    }
} // prints all chromosomes, for each chromosome we call printgene function for printing its genes after we print fitness score stored in gene 0 until we reach last chromosome, at last step we move no further in to recursion
void printgene(genes *data)
{
    if (data->next == NULL)
    {
        printf("%d", data->val);
        return;
    }
    else
    {
        printf("%d:", data->val);
        printgene(data->next);
    }
} // from given gene point on prints until a NULL gene is reached according to assignment paper output structure
void freechromem(chromosome *temp)
{
    if (temp->next == NULL)
    {                            // if we are at last chromosome don't move further in to recursion and return
        freememory(temp->genes); // call gene freeing function
        free(temp);              // free this chromosome link
        return;
    }
    else
    {
        freechromem(temp->next); // first move to next chromosome, last chromosome will be freed first!
        freememory(temp->genes);
        free(temp);
    }
} // main memory freeing function to freeing our main linked link database
void freememory(genes *temp)
{
    if (temp->next == NULL)
    {               // don't call recursion if there are no more links
        free(temp); // freeing this link
        return;
    }
    else
    {
        freememory(temp->next); // first move to last gene, last gene gets freed first
        free(temp);
    }
} // when taken a gene link frees memory until NULL link reached