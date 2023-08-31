/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

#ifndef GA_H
#define GA_H
typedef struct genes
{                       // struct for genes
    int val;            // gene value
    struct genes *next; // next gene
} genes;                // struct for genes
typedef struct chromosome
{
    float rank;              // this is only for storage of rank, is not used in any way.
    struct genes *genes;     // pointer to genes
    struct chromosome *next; // next chromosome
} chromosome;
void calcranks(chromosome *chromo);
chromosome *getbest(chromosome *current, chromosome *comp);
void putranks(chromosome *data, int worst, int best);
chromosome *getworst(chromosome *current, chromosome *comp);
void findgold(chromosome *chromo, chromosome *compare);
void initfrom(chromosome *c1, chromosome *c2);
void copychromo(chromosome *c1, chromosome *c2);
void copyadet(genes *c1, genes *c2, int a);
void crossover(chromosome *chromo, int xs, int xe, int c1, int c2);
void copypart(chromosome *c1, chromosome *c2, int xs, int xe);
void mutate(chromosome *data, int pos);
void sortall(chromosome *chromo, int argv);
int sortgenes(chromosome *data);
void calcfitness(chromosome *data);
int givemenumber(int *total, genes *start);
chromosome *getme(chromosome *chromo, int x);
genes *getmegene(genes *start, int x);
void printchromos(chromosome *chromo);
void printall(chromosome *);
void printgene(genes *);
void freememory(genes *);
void freechromem(chromosome *);
#endif // A3_GA_H
