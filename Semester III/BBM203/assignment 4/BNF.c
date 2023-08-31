#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BNF.h"
// #include <math.h> //using this to test outputs.

/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

int main(int argc)
{ // main function
    if (argc == 1)
    { // checking if our given argument number is valid
        // READING FILES
        FILE *opf;
        opf = fopen("op", "r");
        FILE *pre_opf;
        pre_opf = fopen("pre_op", "r");
        FILE *rel_opf;
        rel_opf = fopen("rel_op", "r");
        FILE *set_opf;
        set_opf = fopen("set_op", "r");
        FILE *varf;
        varf = fopen("var", "r");
        // CHECKING IF READEN FILES ARE VALID
        if (opf == NULL)
        {
            printf("ERROR: Could not read op file ");
        }
        else if (pre_opf == NULL)
        {
            printf("ERROR: Could not read pre_op file ");
        }
        else if (rel_opf == NULL)
        {
            printf("ERROR: Could not read rel_op file ");
        }
        else if (set_opf == NULL)
        {
            printf("ERROR: Could not read set_op file ");
        }
        else if (varf == NULL)
        {
            printf("ERROR: Could not read var file ");
        }
        else
        {
            // COPYING CONTENTS OF FILES TO LINKED-LISTS
            file *op = calloc(1, sizeof(file));
            file *pre_op = calloc(1, sizeof(file));
            file *rel_op = calloc(1, sizeof(file));
            file *set_op = calloc(1, sizeof(file));
            file *var = calloc(1, sizeof(file));
            op->count = 0;
            pre_op->count = 0;
            rel_op->count = 0;
            set_op->count = 0;
            var->count = 0;
            readfile(opf, op);         // this file reading methods makes it way easier to get a random item from the file.
            readfile(pre_opf, pre_op); // line by line they are put in to a linked list, after that we generate a number and mod it with the count which states the total elements in the list and access nth (that random number) element from linked list. 0 for first item, count-1 for last item.
            readfile(rel_opf, rel_op);
            readfile(set_opf, set_op);
            readfile(varf, var);
            // CLOSING FILES
            fclose(opf);
            fclose(pre_opf);
            fclose(rel_opf);
            fclose(set_opf);
            fclose(varf);
            // This is a developer function used to print linked lists holding the readen files.
            /*
            fileprint(op->head,op->count);
            fileprint(pre_op->head,pre_op->count);
            fileprint(rel_op->head,rel_op->count);
            fileprint(set_op->head,set_op->count);
            fileprint(var->head,var->count);
            */
            srand(time(NULL));                                        // setting random seed, output is different every iteration.
            struct child1 *the_beginning = calloc(1, sizeof(child1)); // allocating memory for first main alg node. alg node is a node with 1 child
            the_beginning->type = 11;                                 // type 11 for alg node
            the_beginning->left = calloc(1, sizeof(void *));          // allocating memory for next node pointed by alg node
            child1c(the_beginning, op, pre_op, rel_op, set_op, var);  // putting alg node into 1 childed node generator, 1 childed node generator will determine it is alg node by looking its type (11) and get a random integer, with that random integer it will decide to generate from 2 types of 3 childed cond nodes and so on.
            pchild1(the_beginning);                                   // printing results with putting our alg node to a 1 childed printer, it will determine it is the alg node and in a recursive way decode and access its children and print results
            // BEGINNING OF FREEING MEMORY
            free1(the_beginning); // freeing all nodes except alg node by putting alg node to a 1 childed freer.
            free(the_beginning);  // freeing alg node
            freefile(op->head);   // freeing the linked lists used to store our readen files
            freefile(pre_op->head);
            freefile(rel_op->head);
            freefile(set_op->head);
            freefile(var->head);
            free(op); // freeing the main file holder for readen files
            free(pre_op);
            free(rel_op);
            free(set_op);
            free(var);
        }
    }
    else if (argc > 1)
    {
        printf("ERROR: Too many arguments supplied.\n");
    } // error for too many arguments
    else
    {
        printf("ERROR: Command line arguments are not as expected.\n");
    }         // error for not enough arguments
    return 1; // ending the main, :)
}
// FUNCTIONS FOR CREATING TREE
void child3c(child3 *toassign, file *op, file *pre_op, file *rel_op, file *set_op, file *var)
{
    if (toassign->type == 1)
    {
        // For first child
        int random = rand() % 2;
        if (random == 0)
        {
            toassign->left = realloc(toassign->left, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 1;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->left = realloc(toassign->left, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 2;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child3));
            free(temp);
        }
        else
        {
            printf("while generating left child for a 3 childed node type 1 generated random integer is off the charts");
        }
        // Mid child
        toassign->mid = realloc(toassign->mid, sizeof(child1));
        struct child1 *temp = calloc(1, sizeof(child1));
        temp->type = 6;
        temp->left = calloc(1, sizeof(void *));
        child1c(temp, op, pre_op, rel_op, set_op, var);
        memcpy(toassign->mid, temp, sizeof(child1));
        free(temp);
        random = rand() % 2;
        // Last child
        if (random == 0)
        {
            toassign->right = realloc(toassign->right, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 1;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->right = realloc(toassign->right, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 2;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child3));
            free(temp);
        }
        else
        {
            printf("while generating right child for a 3 childed node type 1 generated random integer is off the charts");
        }
    }
    else if (toassign->type == 2)
    {
        // For first child
        int random = rand() % 3;
        if (random == 0)
        {
            toassign->left = realloc(toassign->left, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 3;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->left = realloc(toassign->left, sizeof(child2));
            struct child2 *temp = calloc(1, sizeof(child2));
            temp->type = 5;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            child2c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child2));
            free(temp);
        }
        else if (random == 2)
        {
            toassign->left = realloc(toassign->left, sizeof(child1));
            struct child1 *temp = calloc(1, sizeof(child1));
            temp->type = 10;
            temp->left = calloc(1, sizeof(void *));
            child1c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child1));
            free(temp);
        }
        else
        {
            printf("while generating left child for a 3 childed node type 2 generated random integer is off the charts");
        }
        // Mid child
        toassign->mid = realloc(toassign->mid, sizeof(child1));
        struct child1 *temp = calloc(1, sizeof(child1));
        temp->type = 7;
        temp->left = calloc(1, sizeof(void *));
        child1c(temp, op, pre_op, rel_op, set_op, var);
        memcpy(toassign->mid, temp, sizeof(child1));
        free(temp);
        random = rand() % 3;
        // Last child
        if (random == 0)
        {
            toassign->right = realloc(toassign->right, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 3;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->right = realloc(toassign->right, sizeof(child2));
            struct child2 *temp = calloc(1, sizeof(child2));
            temp->type = 5;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            child2c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child2));
            free(temp);
        }
        else if (random == 2)
        {
            toassign->right = realloc(toassign->right, sizeof(child1));
            struct child1 *temp = calloc(1, sizeof(child1));
            temp->type = 10;
            temp->left = calloc(1, sizeof(void *));
            child1c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child1));
            free(temp);
        }
        else
        {
            printf("while generating right child for a 3 childed node type 2 generated random integer is off the charts");
        }
    }
    else if (toassign->type == 3)
    {
        // For first child
        int random = rand() % 3;
        if (random == 0)
        {
            toassign->left = realloc(toassign->left, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 3;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->left = realloc(toassign->left, sizeof(child2));
            struct child2 *temp = calloc(1, sizeof(child2));
            temp->type = 5;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            child2c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child2));
            free(temp);
        }
        else if (random == 2)
        {
            toassign->left = realloc(toassign->left, sizeof(child1));
            struct child1 *temp = calloc(1, sizeof(child1));
            temp->type = 10;
            temp->left = calloc(1, sizeof(void *));
            child1c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child1));
            free(temp);
        }
        else
        {
            printf("while generating left child for a 3 childed node type 3 generated random integer is off the charts");
        }
        // Mid child
        toassign->mid = realloc(toassign->mid, sizeof(child1));
        struct child1 *temp = calloc(1, sizeof(child1));
        temp->type = 8;
        temp->left = calloc(1, sizeof(void *));
        child1c(temp, op, pre_op, rel_op, set_op, var);
        memcpy(toassign->mid, temp, sizeof(child1));
        free(temp);
        random = rand() % 3;
        // Last child
        if (random == 0)
        {
            toassign->right = realloc(toassign->right, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 3;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->right = realloc(toassign->right, sizeof(child2));
            struct child2 *temp = calloc(1, sizeof(child2));
            temp->type = 5;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            child2c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child2));
            free(temp);
        }
        else if (random == 2)
        {
            toassign->right = realloc(toassign->right, sizeof(child1));
            struct child1 *temp = calloc(1, sizeof(child1));
            temp->type = 10;
            temp->left = calloc(1, sizeof(void *));
            child1c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->right, temp, sizeof(child1));
            free(temp);
        }
        else
        {
            printf("while generating right child for a 3 childed node type 3 generated random integer is off the charts");
        }
    }
    else
    {
        printf("a 3 childed node has gotten undefined type");
    }
}
void child2c(child2 *toassign, file *op, file *pre_op, file *rel_op, file *set_op, file *var)
{
    if (toassign->type == 5)
    {
        // Left child
        toassign->left = realloc(toassign->left, sizeof(child1));
        struct child1 *temp = calloc(1, sizeof(child1));
        temp->type = 9;
        temp->left = calloc(1, sizeof(void *));
        child1c(temp, op, pre_op, rel_op, set_op, var);
        memcpy(toassign->left, temp, sizeof(child1));
        free(temp);
        // Mid child
        int random = rand() % 3;
        if (random == 0)
        {
            toassign->mid = realloc(toassign->mid, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 3;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->mid, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->mid = realloc(toassign->mid, sizeof(child2));
            struct child2 *temp = calloc(1, sizeof(child2));
            temp->type = 5;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            child2c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->mid, temp, sizeof(child2));
            free(temp);
        }
        else if (random == 2)
        {
            toassign->mid = realloc(toassign->mid, sizeof(child1));
            struct child1 *temp = calloc(1, sizeof(child1));
            temp->type = 10;
            temp->left = calloc(1, sizeof(void *));
            child1c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->mid, temp, sizeof(child1));
            free(temp);
        }
        else
        {
            printf("while generating mid child for a 2 childed node of type 5 generated random integer is off the charts");
        }
    }
    else
    {
        printf("a 2 childed node has gotten undefined type");
    }
}
void child1c(child1 *toassign, file *op, file *pre_op, file *rel_op, file *set_op, file *var)
{
    if (toassign->type == 6)
    {
        toassign->left = realloc(toassign->left, sizeof(child0));
        struct child0 *temp = calloc(1, sizeof(child0));
        temp->type = 0;
        temp->left = strdup(givemenode(set_op));
        memcpy(toassign->left, temp, sizeof(child0));
        free(temp);
    }
    else if (toassign->type == 7)
    {
        toassign->left = realloc(toassign->left, sizeof(child0));
        struct child0 *temp = calloc(1, sizeof(child0));
        temp->type = 0;
        temp->left = strdup(givemenode(rel_op));
        memcpy(toassign->left, temp, sizeof(child0));
        free(temp);
    }
    else if (toassign->type == 8)
    {
        toassign->left = realloc(toassign->left, sizeof(child0));
        struct child0 *temp = calloc(1, sizeof(child0));
        temp->type = 0;
        temp->left = strdup(givemenode(op));
        memcpy(toassign->left, temp, sizeof(child0));
        free(temp);
    }
    else if (toassign->type == 9)
    {
        toassign->left = realloc(toassign->left, sizeof(child0));
        struct child0 *temp = calloc(1, sizeof(child0));
        temp->type = 0;
        temp->left = strdup(givemenode(pre_op));
        memcpy(toassign->left, temp, sizeof(child0));
        free(temp);
    }
    else if (toassign->type == 10)
    {
        toassign->left = realloc(toassign->left, sizeof(child0));
        struct child0 *temp = calloc(1, sizeof(child0));
        temp->type = 0;
        temp->left = strdup(givemenode(var));
        memcpy(toassign->left, temp, sizeof(child0));
        free(temp);
    }
    else if (toassign->type == 11)
    {
        int random = rand() % 2;
        if (random == 0)
        {
            toassign->left = realloc(toassign->left, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 1;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child3));
            free(temp);
        }
        else if (random == 1)
        {
            toassign->left = realloc(toassign->left, sizeof(child3));
            struct child3 *temp = calloc(1, sizeof(child3));
            temp->type = 2;
            temp->left = calloc(1, sizeof(void *));
            temp->mid = calloc(1, sizeof(void *));
            temp->right = calloc(1, sizeof(void *));
            child3c(temp, op, pre_op, rel_op, set_op, var);
            memcpy(toassign->left, temp, sizeof(child3));
            free(temp);
        }
    }
    else
    {
        printf("a 1 childed node has gotten undefined type");
    }
}
// FUNCTION FOR GETTING RANDOM ITEM FROM THE FILE
char *givemenode(file *thefile)
{
    int random = rand() % (thefile->count);
    node *current;
    int i;
    current = thefile->head;
    for (i = 0; i < random; i++)
    {
        current = current->next;
    }
    return current->content;
}
// FUNCTIONS FOR PRINTING THE TREE
void pchild3(child3 *toprint)
{
    // printf("\ncurrent type: %d\n",toprint->type);
    printf("(");
    if (toprint->type == 1)
    {
        pchild3(toprint->left);
        pchild1(toprint->mid);
        pchild3(toprint->right);
    }
    else if (toprint->type == 3 || toprint->type == 2)
    {
        child1 *temp;
        temp = toprint->left;
        if (temp->type == 3)
        {
            pchild3(toprint->left);
        }
        else if (temp->type == 5)
        {
            pchild2(toprint->left);
        }
        else if (temp->type == 10)
        {
            pchild1(toprint->left);
        }
        pchild1(toprint->mid);
        child1 *temp2;
        temp2 = toprint->right;
        if (temp2->type == 3)
        {
            pchild3(toprint->right);
        }
        else if (temp2->type == 5)
        {
            pchild2(toprint->right);
        }
        else if (temp2->type == 10)
        {
            pchild1(toprint->right);
        }
    }
    else
    {
        printf("while printing 3 noded child got undefined type %d", toprint->type);
    }
    printf(")");
}
void pchild2(child2 *toprint)
{
    // printf("\ncurrent type: %d\n",toprint->type);
    if (toprint->type == 5)
    {
        pchild1(toprint->left);
        printf("(");
        child1 *temp;
        temp = toprint->mid;
        if (temp->type == 3)
        {
            pchild3(toprint->mid);
        }
        else if (temp->type == 5)
        {
            pchild2(toprint->mid);
        }
        else if (temp->type == 10)
        {
            pchild1(toprint->mid);
        }
        printf(")");
    }
    else
    {
        printf("while printing a 2 noded child got undefined type %d", toprint->type);
    }
}
void pchild1(child1 *toprint)
{
    // printf("\ncurrent type: %d\n",toprint->type);
    if (toprint->type < 11 && toprint->type >= 5)
    {
        pchild0(toprint->left);
    }
    else if (toprint->type == 11)
    {
        printf("if(");
        pchild3(toprint->left);
        printf(") { }");
    }
    else
    {
        printf("while printing a 1 noded child got undefined type %d", toprint->type);
    }
}
void pchild0(child0 *toprint)
{
    // printf("\ncurrent type: %d\n",toprint->type);
    if (toprint->type == 0)
    {
        printf("%s", toprint->left);
    }
    else
    {
        printf("while printing a 0 noded child got undefined type %d", toprint->type);
    }
}
// FUNCTIONS FOR FREEING THE TREE
void free3(child3 *tofree)
{
    // printf("\nFreeing current type: %d\n",tofree->type);
    if (tofree->type == 1)
    {
        free3(tofree->left);
        free1(tofree->mid);
        free3(tofree->right);
        free(tofree->left);
        free(tofree->mid);
        free(tofree->right);
    }
    else if (tofree->type == 3 || tofree->type == 2)
    {
        child1 *temp;
        temp = tofree->left;
        if (temp->type == 3)
        {
            free3(tofree->left);
        }
        else if (temp->type == 5)
        {
            free2(tofree->left);
        }
        else if (temp->type == 10)
        {
            free1(tofree->left);
        }
        free1(tofree->mid);
        child1 *temp2;
        temp2 = tofree->right;
        if (temp2->type == 3)
        {
            free3(tofree->right);
        }
        else if (temp2->type == 5)
        {
            free2(tofree->right);
        }
        else if (temp2->type == 10)
        {
            free1(tofree->right);
        }
        free(tofree->left);
        free(tofree->mid);
        free(tofree->right);
    }
    else
    {
        printf("while freeing 3 noded child got undefined type %d", tofree->type);
    }
}
void free2(child2 *tofree)
{
    // printf("\nFreeing current type: %d\n",tofree->type);
    if (tofree->type == 5)
    {
        free1(tofree->left);
        child1 *temp;
        temp = tofree->mid;
        if (temp->type == 3)
        {
            free3(tofree->mid);
        }
        else if (temp->type == 5)
        {
            free2(tofree->mid);
        }
        else if (temp->type == 10)
        {
            free1(tofree->mid);
        }
        free(tofree->left);
        free(tofree->mid);
    }
    else
    {
        printf("while freeing a 2 noded child got undefined type %d", tofree->type);
    }
}
void free1(child1 *tofree)
{
    // printf("\nFreeing current type: %d\n",tofree->type);
    if (tofree->type < 11 && tofree->type >= 5)
    {
        free0(tofree->left);
        free(tofree->left);
    }
    else if (tofree->type == 11)
    {
        free3(tofree->left);
        free(tofree->left);
    }
    else
    {
        printf("while freeing a 1 noded child got undefined type %d", tofree->type);
    }
}
void free0(child0 *tofree)
{
    // printf("\nFreeing current type: %d\n",tofree->type);
    if (tofree->type == 0)
    {
        free(tofree->left);
    }
    else
    {
        printf("while freeing a 0 noded child got undefined type %d", tofree->type);
    }
}

void readfile(FILE *from, file *toread)
{
    char *line = calloc(20, sizeof(char)); // our main line string for reading file
    node *curr;
    int len;
    curr = NULL;
    toread->head = NULL;
    while (fgets(line, 20 * sizeof(char), from))
    {
        len = strlen(line);
        if (line[len - 1] == '\n')
        {
            line[len - 1] = 0;
        }
        toread->count = toread->count + 1;
        node *temp = calloc(1, sizeof(node));
        temp->content = strdup(line);
        temp->next = NULL;
        if (toread->head == NULL)
        {
            toread->head = temp;
            curr = toread->head;
        }
        else
        {
            curr->next = temp;
            curr = curr->next;
        }
    }
    free(line);
} // This function reads the files to a linked list. First item has an integer value holding the total items in linked list, rest of the items has char* for the lines in the file.
void fileprint(node *toprint, int count)
{
    printf("%d items:\n", count);
    node *current;
    for (current = toprint; current; current = current->next)
    {
        printf("%s ", current->content);
    }
    printf("\n");
} // This is a developer function used to print linked lists that hold the files.
void freefile(node *tofree)
{
    if (tofree->next == NULL)
    {
        free(tofree->content);
        free(tofree);
    }
    else
    {
        freefile(tofree->next);
        free(tofree->content);
        free(tofree);
    }
} // Function for freeing the linked lists that hold the files.