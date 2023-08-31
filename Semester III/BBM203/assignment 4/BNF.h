#ifndef BNF_H
#define BNF_H

/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

typedef struct child3
{
    int type;
    /* Type 1: (<cond><set-op><cond>) cond
     * Type 2: (<expr><rel-op><expr>) cond
     * Type 3: (<expr><op><expr>) expr
     */
    void *left;
    void *mid;
    void *right;
} child3;
typedef struct child2
{
    int type;
    /* Type 5: <pre-op>(<expr>) expr
     */
    void *left;
    void *mid;
} child2;
typedef struct child1
{
    int type;
    /* Type 6: <set-op>
     * Type 7: <rel-op>
     * Type 8: <op>
     * Type 9: <pre-op>
     * Type 10: <var> expr
     * Type 11: alg
     */
    void *left;
} child1;
typedef struct child0
{
    /* Type 0: char* read
     */
    int type;
    char *left;
} child0;

typedef struct node
{
    char *content;
    struct node *next;
} node;
typedef struct file
{
    int count;
    struct node *head;
} file;

void readfile(FILE *from, file *toread);
void fileprint(node *toprint, int count);
void freefile(node *tofree);

void child3c(child3 *toassign, file *op, file *pre_op, file *rel_op, file *set_op, file *var);
void child2c(child2 *toassign, file *op, file *pre_op, file *rel_op, file *set_op, file *var);
void child1c(child1 *toassign, file *op, file *pre_op, file *rel_op, file *set_op, file *var);

void pchild3(child3 *toprint);
void pchild2(child2 *toprint);
void pchild1(child1 *toprint);
void pchild0(child0 *toprint);

void free3(child3 *tofree);
void free2(child2 *tofree);
void free1(child1 *tofree);
void free0(child0 *tofree);

char *givemenode(file *thefile);

#endif // A4_BNF_H
