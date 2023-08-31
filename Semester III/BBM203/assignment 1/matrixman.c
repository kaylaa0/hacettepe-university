#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

int main(int argc, char *argv[])
{                                                  // main function, we have got only 1 function in my code and it is main function.
    int ***matrises = calloc(101, sizeof(int **)); // allocating 3d array for 2d matrises
    matrises[0] = calloc(1, sizeof(int *));        // allocating our key array for matrix list
    matrises[0][0] = calloc(101, sizeof(int));     // matrix list; usage is simple, if matrix no = 0 matrix is not initialized, if matrix no = matrix no matrix exist.
    int **vectors = calloc(101, sizeof(int *));    // initializing 2d array for vectors
    vectors[0] = calloc(101, sizeof(int));         // our vector list, same as matrix list, it is 0th element of vector array
    if (argc == 4)
    { // checking if our given argument number is valid
        FILE *commands;
        commands = fopen(argv[2], "r"); // reading commands file
        if (commands == NULL)
        {
            printf("ERROR: Could not open commands file ");
        } // checking if commands file is valid
        else
        {
            FILE *output;
            output = fopen(argv[3], "w"); // reading output file
            if (output == NULL)
            {
                printf("ERROR: Could not open output file ");
            } // checking if output file is valid
            else
            {
                char *line = calloc(50, sizeof(char));    // allocating line memory
                char *command = calloc(15, sizeof(char)); // allocating memory for readen command
                int i;                                    // int i, used in many places in the code, initialized here.
                while (fgets(line, 50 * sizeof(char), commands) != NULL)
                { // reaading our file line by line
                    for (i = 0; line[i] != ' '; ++i)
                    {
                        command[i] = line[i];
                    } // Copying command from line
                    // BEGINNING OF MATREAD
                    if (!strcmp(command, "matread"))
                    {
                        char *matrix = calloc(10, sizeof(char));
                        char *lpoint = line;
                        int nlinec, word = 0;
                        char *readen = calloc(10, sizeof(char)); // readen in my code usually means readen word in the command line. I read them word by word and ignore spaces.
                        word++;
                        while (*lpoint)
                        {                                         // This is one of the most used function in my code. I have not initialized seperate functions instead I copy paste them! sorry for making the mess.
                            memset(readen, 0, 10 * sizeof(char)); // we reset our word in case of memory flows after a bigger word being readen.
                            for (; *lpoint; lpoint++)
                            {
                                if (*lpoint >= 'a' && *lpoint <= 'z')
                                { // for some of the functions this command also reads numbers. for matread no word has to start with number so we are checking for alphabet only
                                    break;
                                }
                            }
                            if (sscanf(lpoint, "%s%n", readen, &nlinec) == 1)
                            { // reading the found alphabet pointer until we get null.
                                if (word == 2)
                                { // now we checking word number, and putting matching word in to matching pointers, however for this command there is only 1 word so we will read it directly from readen so we break
                                    lpoint += nlinec;
                                    break;
                                }
                            }
                            lpoint += nlinec;
                            word++;
                        }
                        if (readen[0] == 'm')
                        { // checking if the command is like matread v12.mat or something
                            for (i = 1; readen[i] != '.'; i++)
                            { // reading until . so we have matrix name
                                matrix[i - 1] = readen[i];
                            }
                            strcat(matrix, "\0");
                            if (matrises[0][0][atoi(matrix)] != 0)
                            { // checking if the given matrix already initialized, if it is printing error.
                                fprintf(output, "error\n");
                            }
                            else
                            {
                                char *matread = calloc(1, strlen(argv[1]) + sizeof(matrix) + sizeof(char) * 3 + 1); // we are getting ready to read our mat file, this will be the file to read.
                                strcat(matread, argv[1]);                                                           // adding argv
                                strcat(matread, readen);                                                            // and adding the file name
                                FILE *matfile = fopen(matread, "r");                                                // reading matfile
                                if (matfile == NULL)
                                {
                                    fprintf(output, "error\n");
                                }
                                else
                                {
                                    char *matline = calloc(100, sizeof(char)); // the line pointer we are gonna read lines from matfile to
                                    int a, c, column, row;
                                    a = 1;
                                    column = 0;
                                    row = 0;
                                    char *linearray = calloc(100, sizeof(char)); // this is a dummy line array, before we actually read the matfile, i wanted to get the size so we quickly take a peek to the matfile with this pointer
                                    memset(linearray, 0, 100 * sizeof(char));
                                    while (fgets(linearray, 100 * sizeof(char), matfile) != NULL)
                                    {
                                        row++;
                                    } // counting rows of matfile
                                    memset(linearray, 0, 100 * sizeof(char));
                                    fseek(matfile, 0L, SEEK_SET);
                                    while (fgets(linearray, 100 * sizeof(char), matfile) != NULL)
                                    {
                                        break;
                                    }
                                    char *point = linearray;
                                    int val, nchars = 0;
                                    while (*point)
                                    { // in this dummy function we are quickly counting rows.
                                        for (; *point; point++)
                                        {
                                            if (*point >= '0' && *point <= '9')
                                            {
                                                column++;
                                                break;
                                            }
                                            if (*point == '-' && *(point + 1) >= '0' && *(point + 1) <= '9')
                                            {
                                                column++;
                                                break;
                                            }
                                        }
                                        if (sscanf(point, "%d%n", &val, &nchars) == 1)
                                        {
                                        }
                                        point += nchars;
                                    }
                                    memset(linearray, 0, 100 * sizeof(char));
                                    free(linearray);
                                    fseek(matfile, 0L, SEEK_SET);
                                    matrises[atoi(matrix)] = calloc(row + 1, sizeof(int *)); // initializing second dimension of matrix
                                    matrises[atoi(matrix)][0] = calloc(2, sizeof(int));      // initializing matrix key, this is also important part of the code.
                                    matrises[atoi(matrix)][0][0] = row;                      // it stores 2 numbers, row and column. So every matrix has its size stored at 0th array.
                                    matrises[atoi(matrix)][0][1] = column;
                                    while (fgets(matline, 100 * sizeof(char), matfile) != NULL)
                                    { // reading matfile
                                        c = 0;
                                        char *pointer = matline;
                                        int value, ncharss = 0;
                                        if (a <= row)
                                        {                                                                // this is fix to a bug, I think it doesn't exist anymore but still keeping things safe.
                                            matrises[atoi(matrix)][a] = calloc(column + 1, sizeof(int)); // allocating third dimension which is the last dimension for our array
                                        }
                                        while (*pointer)
                                        { // this function is gonna set the values
                                            for (; *pointer; pointer++)
                                            {
                                                if (*pointer >= '0' && *pointer <= '9')
                                                {
                                                    c++;
                                                    break;
                                                }
                                                if (*pointer == '-' && *(pointer + 1) >= '0' && *(pointer + 1) <= '9')
                                                {
                                                    c++;
                                                    break;
                                                }
                                            }
                                            if (sscanf(pointer, "%d%n", &value, &ncharss) == 1)
                                            {
                                                if (a <= row)
                                                {
                                                    matrises[atoi(matrix)][a][c] = value; // setting values
                                                }
                                            }
                                            pointer += ncharss;
                                        }
                                        memset(matline, 0, 100 * sizeof(char));
                                        a++;
                                    }
                                    memset(matline, 0, 100 * sizeof(char));
                                    free(matline);
                                    matrises[0][0][atoi(matrix)] = atoi(matrix);
                                    fprintf(output, "read matrix %s %d %d\n", readen, matrises[atoi(matrix)][0][0], matrises[atoi(matrix)][0][1]); // printing the result to ouput. After every function there is one of this command for printing. I am terribly sorry that I didn't create a seperate function for this.
                                    for (i = 1; i <= matrises[atoi(matrix)][0][0]; i++)
                                    {
                                        for (c = 1; c <= matrises[atoi(matrix)][0][1]; c++)
                                        {
                                            if (c < matrises[atoi(matrix)][0][1])
                                            {
                                                fprintf(output, "%d ", matrises[atoi(matrix)][i][c]); // printing..
                                            }
                                            else if (c = matrises[atoi(matrix)][0][1])
                                            {
                                                fprintf(output, "%d\n", matrises[atoi(matrix)][i][c]); // printing...
                                            }
                                        }
                                    }
                                    memset(matread, 0, strlen(argv[1]) + sizeof(matrix) + sizeof(char) * 3 + 1); // setting our matread array to 0, every array is set to 0 before freeing them but I never use malloc I always use calloc, you can never be too safe lol!
                                    free(matread);                                                               // freeing memory
                                    fclose(matfile);
                                }
                            }                                     // closing matfile
                            memset(readen, 0, 10 * sizeof(char)); // i know these commands are at the end of every function, i am sorry again that i have not initialized any seperate functions, but the cause of that is everything is customized a little for the function.
                            free(readen);
                            memset(matrix, 0, 10 * sizeof(char));
                            free(matrix);
                        }
                        else
                        {
                            memset(readen, 0, 10 * sizeof(char)); // freeing our memory even the function did not work.
                            free(readen);
                            fprintf(output, "error\n");
                            memset(matrix, 0, 10 * sizeof(char));
                            free(matrix);
                        }
                    } // BEGINNING OF VECREAD
                    else if (!strcmp(command, "vecread"))
                    {
                        char *vector = calloc(10, sizeof(char)); // from now on all the functions share same structure, even some of them are so similar only a few chracters are different.
                        char *lpoint = line;
                        int nlinec, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*lpoint)
                        {
                            memset(readen, 0, 10 * sizeof(char));
                            for (; *lpoint; lpoint++)
                            {
                                if (*lpoint >= 'a' && *lpoint <= 'z')
                                {
                                    break;
                                }
                            }
                            if (sscanf(lpoint, "%s%n", readen, &nlinec) == 1)
                            {
                                if (word == 2)
                                {
                                    lpoint += nlinec;
                                    break;
                                }
                            }
                            lpoint += nlinec;
                            word++;
                        }
                        if (readen[0] == 'v')
                        {
                            for (i = 1; readen[i] != '.'; i++)
                            {
                                vector[i - 1] = readen[i];
                            }
                            strcat(vector, "\0");
                            if (vectors[0][atoi(vector)] != 0)
                            {
                                fprintf(output, "error\n"); // every function has its different error checks.
                            }
                            else
                            {
                                char *vecread = calloc(1, strlen(argv[1]) + sizeof(vector) + sizeof(char) * 3 + 1);
                                strcat(vecread, argv[1]);
                                strcat(vecread, readen);
                                FILE *vecfile = fopen(vecread, "r");
                                if (vecfile == NULL)
                                {
                                    fprintf(output, "error\n");
                                }
                                else
                                {
                                    char *vecline = calloc(100, sizeof(char));
                                    int column;
                                    column = 0;
                                    char *linearray2 = calloc(100, sizeof(char));
                                    while (fgets(linearray2, 100 * sizeof(char), vecfile) != NULL)
                                    {
                                        char *pointo = linearray2;
                                        int val, nchars = 0;
                                        while (*pointo)
                                        {
                                            for (; *pointo; pointo++)
                                            {
                                                if (*pointo >= '0' && *pointo <= '9')
                                                {
                                                    column++;
                                                    break;
                                                }
                                                if (*pointo == '-' && *(pointo + 1) >= '0' && *(pointo + 1) <= '9')
                                                {
                                                    column++;
                                                    break;
                                                }
                                            }
                                            if (sscanf(pointo, "%d%n", &val, &nchars) == 1)
                                            {
                                            }
                                            pointo += nchars;
                                        }
                                        break;
                                        memset(linearray2, 0, 100 * sizeof(char));
                                    }
                                    memset(linearray2, 0, 100 * sizeof(char));
                                    free(linearray2);
                                    vectors[atoi(vector)] = calloc(column + 1, sizeof(int)); // allocating memory for the vector
                                    vectors[atoi(vector)][0] = column;                       // again we have vector key, which stores the size of vector.
                                    int n;
                                    n = 0;
                                    fseek(vecfile, 0L, SEEK_SET);
                                    while (fgets(vecline, 100 * sizeof(char), vecfile) != NULL)
                                    {
                                        char *pointer = vecline;
                                        int value, ncharss = 0;
                                        while (*pointer)
                                        {
                                            for (; *pointer; pointer++)
                                            {
                                                if (*pointer >= '0' && *pointer <= '9')
                                                {
                                                    n++;
                                                    break;
                                                }
                                                if (*pointer == '-' && *(pointer + 1) >= '0' && *(pointer + 1) <= '9')
                                                {
                                                    n++;
                                                    break;
                                                }
                                            }
                                            if (sscanf(pointer, "%d%n", &value, &ncharss) == 1)
                                            {
                                                vectors[atoi(vector)][n] = value;
                                            }
                                            pointer += ncharss;
                                        }
                                        memset(vecline, 0, 100 * sizeof(char));
                                    }
                                    vectors[0][atoi(vector)] = atoi(vector);
                                    fprintf(output, "read vector %s %d\n", readen, vectors[atoi(vector)][0]);
                                    for (i = 1; i <= vectors[atoi(vector)][0]; i++)
                                    {
                                        if (i < vectors[atoi(vector)][0])
                                        {
                                            fprintf(output, "%d ", vectors[atoi(vector)][i]);
                                        }
                                        else
                                        {
                                            fprintf(output, "%d\n", vectors[atoi(vector)][i]);
                                        }
                                    }
                                    memset(vecread, 0, strlen(argv[1]) + sizeof(vector) + sizeof(char) * 3 + 1);
                                    free(vecread);
                                    memset(vecline, 0, 100 * sizeof(char));
                                    free(vecline);
                                    fclose(vecfile);
                                }
                            }
                            memset(readen, 0, 10 * sizeof(char));
                            free(readen);
                            memset(vector, 0, 10 * sizeof(char));
                            free(vector);
                        }
                        else
                        {
                            free(lpoint);
                            memset(readen, 0, 10 * sizeof(char));
                            free(readen);
                            memset(vector, 0, 10 * sizeof(char));
                            free(vector);
                            fprintf(output, "error\n");
                        }
                    } // BEGINNING OF MATZEROS
                    else if (!strcmp(command, "matzeros"))
                    {
                        char *matrix = calloc(5, sizeof(char)); // another function, same structure.
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
                            for (; *point; point++)
                            {
                                if (*point >= 'a' && *point <= 'z')
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    point += nchars;
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        if (readen[0] == 'm')
                        {
                            for (i = 1; i < 6; i++)
                            {
                                matrix[i - 1] = readen[i];
                            }
                            strcat(matrix, "\0");
                            if (matrises[0][0][atoi(matrix)] != 0)
                            {
                                fprintf(output, "error\n");
                            }
                            else
                            {
                                int row, column;
                                while (*point)
                                {
                                    for (; *point; point++)
                                    {
                                        if (*point >= '0' && *point <= '9')
                                        {
                                            break;
                                        }
                                    }
                                    if (sscanf(point, "%d%n", &row, &nchars) == 1)
                                    {
                                    }
                                    point += nchars;
                                    break;
                                }
                                // for(i = 11+strlen(matrix); line[i]!=' '; ++i){row[i-11-strlen(matrix)] = line[i];}   ah i see an old code here that I am not using anymore. When I was coding this assignment I struggled a lot at the beginning and dumped days worth of code. this remains here as a charm.
                                // strcat(row,"\0");
                                while (*point)
                                {
                                    for (; *point; point++)
                                    {
                                        if (*point >= '0' && *point <= '9')
                                        {
                                            break;
                                        }
                                    }
                                    if (sscanf(point, "%d%n", &column, &nchars) == 1)
                                    {
                                    }
                                    point += nchars;
                                    break;
                                }
                                // for(i = 12+strlen(matrix)+strlen(row); line[i+1]!='\n'; ++i){column[i-12-strlen(matrix)-strlen(row)] = line[i];}
                                // strcat(column,"\0");
                                matrises[atoi(matrix)] = calloc(row + 1, sizeof(int *));
                                matrises[atoi(matrix)][0] = calloc(2, sizeof(int));
                                matrises[atoi(matrix)][0][0] = row;
                                matrises[atoi(matrix)][0][1] = column;
                                row++;
                                column++;
                                int a, b;
                                for (a = 1; a < row; a++)
                                {
                                    matrises[atoi(matrix)][a] = calloc(column, sizeof(int));
                                    for (b = 1; b < column; b++)
                                    {
                                        matrises[atoi(matrix)][a][b] = 0;
                                    }
                                }
                                matrises[0][0][atoi(matrix)] = atoi(matrix);
                                fprintf(output, "created matrix m%s %d %d\n", matrix, matrises[atoi(matrix)][0][0], matrises[atoi(matrix)][0][1]);
                                for (i = 1; i <= matrises[atoi(matrix)][0][0]; i++)
                                {
                                    for (b = 1; b < matrises[atoi(matrix)][0][1]; b++)
                                    {
                                        fprintf(output, "%d ", matrises[atoi(matrix)][i][b]);
                                    }
                                    fprintf(output, "%d\n", matrises[atoi(matrix)][i][b]);
                                }
                            }
                            memset(readen, 0, 10 * sizeof(char));
                            free(readen);
                            memset(matrix, 0, 5 * sizeof(char));
                            free(matrix);
                        }
                        else
                        {
                            memset(matrix, 0, 5 * sizeof(char));
                            free(matrix);
                            memset(readen, 0, 10 * sizeof(char));
                            free(readen);
                            fprintf(output, "error\n");
                        }
                    } // BEGINNING OF VECZEROS/
                    else if (!strcmp(command, "veczeros"))
                    {
                        char *vector = calloc(5, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
                            for (; *point; point++)
                            {
                                if (*point >= 'a' && *point <= 'z')
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            {
                                if (word == 2)
                                {
                                    point += nchars;
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        if (readen[0] == 'v')
                        {
                            for (i = 1; i < 6; i++)
                            {
                                vector[i - 1] = readen[i];
                            }
                            strcat(vector, "\0");
                            if (vectors[0][atoi(vector)] != 0)
                            {
                                fprintf(output, "error\n");
                            }
                            else
                            {
                                int column;
                                while (*point)
                                {
                                    for (; *point; point++)
                                    {
                                        if (*point >= '0' && *point <= '9')
                                        {
                                            break;
                                        }
                                    }
                                    if (sscanf(point, "%d%n", &column, &nchars) == 1)
                                    {
                                    }
                                    point += nchars;
                                    break;
                                }
                                // for(i = 11+strlen(vector); line[i+1]!='\n'; ++i){column[i-11-strlen(vector)] = line[i];}
                                vectors[atoi(vector)] = calloc(column + 1, sizeof(int));
                                vectors[atoi(vector)][0] = column;
                                column++;
                                int a;
                                for (a = 1; a < column; a++)
                                {
                                    vectors[atoi(vector)][a] = 0;
                                }
                                vectors[0][atoi(vector)] = atoi(vector);
                                fprintf(output, "created vector %s %d\n", readen, vectors[atoi(vector)][0]);
                                for (i = 1; i <= vectors[atoi(vector)][0]; i++)
                                {
                                    if (i < vectors[atoi(vector)][0])
                                    {
                                        fprintf(output, "%d ", vectors[atoi(vector)][i]);
                                    }
                                    else if (i == vectors[atoi(vector)][0])
                                    {
                                        fprintf(output, "%d\n", vectors[atoi(vector)][i]);
                                    }
                                }
                                memset(readen, 0, 10 * sizeof(char));
                                free(readen);
                                memset(vector, 0, 5 * sizeof(char));
                                free(vector);
                            }
                        }
                        else
                        {
                            memset(readen, 0, 10 * sizeof(char));
                            free(readen);
                            memset(vector, 0, 5 * sizeof(char));
                            free(vector);
                            fprintf(output, "error\n");
                        }
                    } // BEGINNING OF VECSTACK
                    else if (!strcmp(command, "vecstack"))
                    {
                        char *vector1 = calloc(5, sizeof(char));
                        char *vector2 = calloc(5, sizeof(char));
                        char *matrix = calloc(5, sizeof(char));
                        char *direction = calloc(10, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9'))
                                {
                                    break;
                                }
                            }
                            if (sscanf(point, "%s%n", readen, &nchars) == 1)
                            { // lots of words here and we check their number and store them according to that.
                                if (word == 2)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        vector1[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        vector2[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        direction[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        matrix[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *vector1no = calloc(4, sizeof(char)); // for every vector or matrix stored there is also matrixno.
                        char *vector2no = calloc(4, sizeof(char));
                        char *matrixno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            vector1no[i - 1] = vector1[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            vector2no[i - 1] = vector2[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        if (vector1[0] == 'v' && vector2[0] == 'v' && matrix[0] == 'm')
                        {
                            if (vectors[0][atoi(vector1no)] != 0 && vectors[0][atoi(vector2no)] != 0 && matrises[0][0][atoi(matrixno)] == 0)
                            {
                                if (vectors[atoi(vector1no)][0] == vectors[atoi(vector2no)][0])
                                {
                                    if (!strcmp(direction, "row"))
                                    {
                                        matrises[atoi(matrixno)] = calloc(3, sizeof(int *));
                                        matrises[0][0][atoi(matrixno)] = atoi(matrixno);
                                        matrises[atoi(matrixno)][0] = calloc(2, sizeof(int));
                                        matrises[atoi(matrixno)][0][0] = 2;
                                        matrises[atoi(matrixno)][0][1] = vectors[atoi(vector1no)][0];
                                        matrises[atoi(matrixno)][1] = calloc((vectors[atoi(vector1no)][0]) + 1, sizeof(int));
                                        matrises[atoi(matrixno)][2] = calloc((vectors[atoi(vector2no)][0]) + 1, sizeof(int));
                                        for (i = 1; i <= vectors[atoi(vector1no)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][1][i] = vectors[atoi(vector1no)][i];
                                        }
                                        for (i = 1; i <= vectors[atoi(vector2no)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][2][i] = vectors[atoi(vector2no)][i];
                                        }
                                        fprintf(output, "vectors concatenated %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                        int a, b;
                                        for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                                }
                                            }
                                        }
                                    }
                                    else if (!strcmp(direction, "column"))
                                    {
                                        matrises[0][0][atoi(matrixno)] = atoi(matrixno);
                                        matrises[atoi(matrixno)] = calloc((vectors[atoi(vector1no)][0]) + 1, sizeof(int *));
                                        matrises[atoi(matrixno)][0] = calloc(2, sizeof(int));
                                        matrises[atoi(matrixno)][0][0] = vectors[atoi(vector1no)][0];
                                        matrises[atoi(matrixno)][0][1] = 2;
                                        for (i = 1; i <= vectors[atoi(vector1no)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i] = calloc(3, sizeof(int));
                                        }
                                        for (i = 1; i <= vectors[atoi(vector1no)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i][1] = vectors[atoi(vector1no)][i];
                                        }
                                        for (i = 1; i <= vectors[atoi(vector2no)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i][2] = vectors[atoi(vector2no)][i];
                                        }
                                        fprintf(output, "vectors concatenated %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                        int a, b;
                                        for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                                }
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
                        memset(vector1, 0, 5 * sizeof(char)); // freeing all used arrays
                        free(vector1);
                        memset(vector1no, 0, 4 * sizeof(char));
                        free(vector1no);
                        memset(vector2, 0, 5 * sizeof(char));
                        free(vector2);
                        memset(vector2no, 0, 4 * sizeof(char));
                        free(vector2no);
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(direction, 0, 10 * sizeof(char));
                        free(direction);
                        memset(readen, 0, 10 * sizeof(char));
                        free(readen);
                    } // BEGINNINGE OF VECSLICE
                    else if (!strcmp(command, "vecslice"))
                    {
                        char *vector = calloc(5, sizeof(char));
                        char *start = calloc(5, sizeof(char));
                        char *stop = calloc(5, sizeof(char));
                        char *name = calloc(5, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
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
                                        vector[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        start[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        stop[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        name[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *vectorno = calloc(4, sizeof(char));
                        char *nameno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            vectorno[i - 1] = vector[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            nameno[i - 1] = name[i];
                        }
                        if (vector[0] == 'v' && name[0] == 'v')
                        {
                            if (vectors[0][atoi(vectorno)] != 0 && vectors[0][atoi(nameno)] == 0)
                            {
                                if (vectors[atoi(vectorno)][0] >= atoi(start) >= 0 && vectors[atoi(vectorno)][0] >= atoi(stop) >= 0)
                                {
                                    if (atoi(start) != atoi(stop))
                                    {
                                        if (atoi(start) < atoi(stop))
                                        {
                                            vectors[atoi(nameno)] = calloc((atoi(stop) - atoi(start)) + 1, sizeof(int));
                                            vectors[0][atoi(nameno)] = atoi(nameno);
                                            vectors[atoi(nameno)][0] = atoi(stop) - atoi(start);
                                            for (i = atoi(start) + 1; i < atoi(stop) + 1; i++)
                                            {
                                                vectors[atoi(nameno)][i - atoi(start)] = vectors[atoi(vectorno)][i];
                                            }
                                            fprintf(output, "vector sliced %s %d\n", name, vectors[atoi(nameno)][0]);
                                            for (i = 1; i <= vectors[atoi(nameno)][0]; i++)
                                            {
                                                if (i < vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d ", vectors[atoi(nameno)][i]);
                                                }
                                                else if (i == vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d\n", vectors[atoi(nameno)][i]);
                                                }
                                            }
                                        }
                                        else if (atoi(start) > atoi(stop))
                                        {
                                            vectors[atoi(nameno)] = calloc((atoi(start) - atoi(stop)) + 1, sizeof(int));
                                            vectors[0][atoi(nameno)] = atoi(nameno);
                                            vectors[atoi(nameno)][0] = atoi(start) - atoi(stop);
                                            for (i = atoi(start) + 1; i > atoi(stop) + 1; i--)
                                            {
                                                vectors[atoi(nameno)][i - atoi(start)] = vectors[atoi(vectorno)][i];
                                            }
                                            fprintf(output, "vector sliced %s %d\n", name, vectors[atoi(nameno)][0]);
                                            for (i = 1; i <= vectors[atoi(nameno)][0]; i++)
                                            {
                                                if (i < vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d ", vectors[atoi(nameno)][i]);
                                                }
                                                else if (i == vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d\n", vectors[atoi(nameno)][i]);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            fprintf(output, "error\n"); // lots of error checks.
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
                        memset(vectorno, 0, 4 * sizeof(char));
                        free(vectorno);
                        memset(nameno, 0, 4 * sizeof(char));
                        free(nameno);
                        memset(vector, 0, 5 * sizeof(char));
                        free(vector);
                        memset(start, 0, 5 * sizeof(char));
                        free(start);
                        memset(stop, 0, 5 * sizeof(char));
                        free(stop);
                        memset(name, 0, 5 * sizeof(char));
                        free(name);
                        memset(readen, 0, 10 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF MVSTACK
                    else if (!strcmp(command, "mvstack"))
                    {
                        char *matrix = calloc(5, sizeof(char));
                        char *vector = calloc(5, sizeof(char));
                        char *where = calloc(5, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
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
                                        matrix[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        vector[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        where[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrixno = calloc(4, sizeof(char));
                        char *vectorno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            vectorno[i - 1] = vector[i];
                        }
                        if (matrix[0] == 'm' && vector[0] == 'v')
                        {
                            if (matrises[0][0][atoi(matrixno)] != 0 && vectors[0][atoi(vectorno)] != 0)
                            {
                                if (*where == 'r')
                                {
                                    if (matrises[atoi(matrixno)][0][0] == vectors[atoi(vectorno)][0])
                                    {
                                        for (i = 1; i <= matrises[atoi(matrixno)][0][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i] = realloc(matrises[atoi(matrixno)][i], ((matrises[atoi(matrixno)][0][1]) + 2) * sizeof(int));
                                        }
                                        for (i = 1; i <= vectors[atoi(vectorno)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i][(matrises[atoi(matrixno)][0][1]) + 1] = vectors[atoi(vectorno)][i];
                                        }
                                        matrises[atoi(matrixno)][0][1] = (matrises[atoi(matrixno)][0][1]) + 1;
                                        fprintf(output, "matrix and vector concatenated %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                        int a, b;
                                        for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        fprintf(output, "error\n");
                                    }
                                }
                                else if (*where == 'd')
                                {
                                    if (matrises[atoi(matrixno)][0][1] == vectors[atoi(vectorno)][0])
                                    {
                                        matrises[atoi(matrixno)] = realloc(matrises[atoi(matrixno)], ((matrises[atoi(matrixno)][0][0]) + 2) * sizeof(int *));
                                        matrises[atoi(matrixno)][(matrises[atoi(matrixno)][0][0]) + 1] = calloc((matrises[atoi(matrixno)][0][1]) + 1, sizeof(int));
                                        for (i = 1; i <= vectors[atoi(vectorno)][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][(matrises[atoi(matrixno)][0][0]) + 1][i] = vectors[atoi(vectorno)][i];
                                        }
                                        matrises[atoi(matrixno)][0][0] = (matrises[atoi(matrixno)][0][0]) + 1;
                                        fprintf(output, "matrix and vector concatenated %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                        int a, b;
                                        for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                                }
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
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(vectorno, 0, 4 * sizeof(char));
                        free(vectorno);
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(vector, 0, 5 * sizeof(char));
                        free(vector);
                        memset(where, 0, 5 * sizeof(char));
                        free(where);
                        memset(readen, 0, 10 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF MATSTACK
                    else if (!strcmp(command, "matstack"))
                    {
                        char *matrix1 = calloc(5, sizeof(char));
                        char *matrix2 = calloc(5, sizeof(char));
                        char *where = calloc(5, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
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
                                        matrix1[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        matrix2[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        where[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrix1no = calloc(4, sizeof(char));
                        char *matrix2no = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrix1no[i - 1] = matrix1[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            matrix2no[i - 1] = matrix2[i];
                        }
                        if (matrix1[0] == 'm' && matrix2[0] == 'm')
                        {
                            if (matrises[0][0][atoi(matrix1no)] != 0 && matrises[0][0][atoi(matrix2no)] != 0)
                            {
                                if (*where == 'r')
                                {
                                    if (matrises[atoi(matrix1no)][0][0] == matrises[atoi(matrix2no)][0][0])
                                    {
                                        for (i = 1; i <= matrises[atoi(matrix1no)][0][0]; i++)
                                        {
                                            matrises[atoi(matrix1no)][i] = realloc(matrises[atoi(matrix1no)][i], ((matrises[atoi(matrix1no)][0][1]) + (matrises[atoi(matrix2no)][0][1]) + 1) * sizeof(int));
                                        }
                                        int a, b;
                                        for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrix2no)][0][1]; b++)
                                            {
                                                matrises[atoi(matrix1no)][a][(matrises[atoi(matrix1no)][0][1]) + b] = matrises[atoi(matrix2no)][a][b];
                                            }
                                        }
                                        matrises[atoi(matrix1no)][0][1] = (matrises[atoi(matrix1no)][0][1]) + (matrises[atoi(matrix2no)][0][1]);
                                        fprintf(output, "matrices concatenated %s %d %d\n", matrix1, matrises[atoi(matrix1no)][0][0], matrises[atoi(matrix1no)][0][1]);
                                        for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrix1no)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrix1no)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrix1no)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrix1no)][a][b]);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        fprintf(output, "error\n");
                                    }
                                }
                                else if (*where == 'd')
                                {
                                    if (matrises[atoi(matrix1no)][0][1] == matrises[atoi(matrix2no)][0][1])
                                    {
                                        matrises[atoi(matrix1no)] = realloc(matrises[atoi(matrix1no)], ((matrises[atoi(matrix1no)][0][0]) + (matrises[atoi(matrix2no)][0][0]) + 1) * sizeof(int *));
                                        for (i = 1; i <= matrises[atoi(matrix2no)][0][0]; i++)
                                        {
                                            matrises[atoi(matrix1no)][(matrises[atoi(matrix1no)][0][0]) + i] = calloc((matrises[atoi(matrix1no)][0][1]) + 1, sizeof(int));
                                        }
                                        int a, b;
                                        for (a = 1; a <= matrises[atoi(matrix2no)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                            {
                                                matrises[atoi(matrix1no)][a + (matrises[atoi(matrix1no)][0][0])][b] = matrises[atoi(matrix2no)][a][b];
                                            }
                                        }
                                        matrises[atoi(matrix1no)][0][0] = (matrises[atoi(matrix1no)][0][0]) + (matrises[atoi(matrix2no)][0][0]);
                                        fprintf(output, "matrices concatenated %s %d %d\n", matrix1, matrises[atoi(matrix1no)][0][0], matrises[atoi(matrix1no)][0][1]);
                                        for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrix1no)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrix1no)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrix1no)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrix1no)][a][b]);
                                                }
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
                        memset(matrix1no, 0, 4 * sizeof(char));
                        free(matrix1no);
                        memset(matrix2no, 0, 4 * sizeof(char));
                        free(matrix2no);
                        memset(matrix1, 0, 5 * sizeof(char));
                        free(matrix1);
                        memset(matrix2, 0, 5 * sizeof(char));
                        free(matrix2);
                        memset(where, 0, 5 * sizeof(char));
                        free(where);
                        memset(readen, 0, 10 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF PAD
                    else if (!strcmp(command, "pad"))
                    {
                        char *matrix = calloc(5, sizeof(char));
                        char *row = calloc(5, sizeof(char));
                        char *column = calloc(5, sizeof(char));
                        char *mode = calloc(10, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
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
                                        matrix[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        row[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        column[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        mode[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrixno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        if (row[0] == '-')
                        {
                            for (i = 1; i <= 4; i++)
                            {
                                row[i - 1] = row[i];
                            }
                        }
                        if (column[0] == '-')
                        {
                            for (i = 1; i <= 4; i++)
                            {
                                column[i - 1] = column[i];
                            }
                        }
                        if (matrix[0] == 'm')
                        {
                            if (matrises[0][0][atoi(matrixno)] != 0)
                            {
                                if (atoi(row) >= 0 && atoi(column) >= 0)
                                {
                                    if (!strcmp(mode, "maximum"))
                                    {
                                        int val = 0;
                                        matrises[atoi(matrixno)] = realloc(matrises[atoi(matrixno)], ((matrises[atoi(matrixno)][0][0]) + atoi(row) + 1) * sizeof(int *));
                                        for (i = 1; i <= matrises[atoi(matrixno)][0][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i] = realloc(matrises[atoi(matrixno)][i], ((matrises[atoi(matrixno)][0][1]) + atoi(column) + 1) * sizeof(int));
                                        }
                                        int a, b;
                                        for (a = (matrises[atoi(matrixno)][0][0]) + 1; a <= (matrises[atoi(matrixno)][0][0]) + atoi(row); a++)
                                        {
                                            matrises[atoi(matrixno)][a] = calloc((matrises[atoi(matrixno)][0][1]) + atoi(column) + 1, sizeof(int));
                                        }
                                        for (a = 1; a <= (matrises[atoi(matrixno)][0][0]); a++)
                                        {
                                            for (b = (matrises[atoi(matrixno)][0][1]) + 1; b <= (matrises[atoi(matrixno)][0][1]) + atoi(column); b++)
                                            {
                                                val = 0;
                                                for (i = 1; i <= matrises[atoi(matrixno)][0][1]; i++)
                                                {
                                                    if ((matrises[atoi(matrixno)][a][i]) > val)
                                                    {
                                                        val = matrises[atoi(matrixno)][a][i];
                                                    }
                                                    matrises[atoi(matrixno)][a][b] = val;
                                                }
                                            }
                                        }
                                        for (a = (matrises[atoi(matrixno)][0][0]) + 1; a <= (matrises[atoi(matrixno)][0][0]) + atoi(row); a++)
                                        {
                                            for (b = 1; b <= (matrises[atoi(matrixno)][0][1]) + atoi(column); b++)
                                            {
                                                val = 0;
                                                for (i = 1; i <= matrises[atoi(matrixno)][0][0]; i++)
                                                {
                                                    if ((matrises[atoi(matrixno)][i][b]) > val)
                                                    {
                                                        val = matrises[atoi(matrixno)][i][b];
                                                    }
                                                    matrises[atoi(matrixno)][a][b] = val;
                                                }
                                            }
                                        }
                                        matrises[atoi(matrixno)][0][0] = (matrises[atoi(matrixno)][0][0]) + atoi(row);
                                        matrises[atoi(matrixno)][0][1] = (matrises[atoi(matrixno)][0][1]) + atoi(column);
                                        fprintf(output, "matrix paded %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                        for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                                }
                                            }
                                        }
                                    }
                                    else if (!strcmp(mode, "minimum"))
                                    {
                                        int val = 0;
                                        matrises[atoi(matrixno)] = realloc(matrises[atoi(matrixno)], ((matrises[atoi(matrixno)][0][0]) + atoi(row) + 1) * sizeof(int *));
                                        for (i = 1; i <= matrises[atoi(matrixno)][0][0]; i++)
                                        {
                                            matrises[atoi(matrixno)][i] = realloc(matrises[atoi(matrixno)][i], ((matrises[atoi(matrixno)][0][1]) + atoi(column) + 1) * sizeof(int));
                                        }
                                        int a, b;
                                        for (a = (matrises[atoi(matrixno)][0][0]) + 1; a <= (matrises[atoi(matrixno)][0][0]) + atoi(row); a++)
                                        {
                                            matrises[atoi(matrixno)][a] = calloc((matrises[atoi(matrixno)][0][1]) + atoi(column) + 1, sizeof(int));
                                        }
                                        for (a = 1; a <= (matrises[atoi(matrixno)][0][0]); a++)
                                        {
                                            for (b = (matrises[atoi(matrixno)][0][1]) + 1; b <= (matrises[atoi(matrixno)][0][1]) + atoi(column); b++)
                                            {
                                                val = matrises[atoi(matrixno)][a][b - (matrises[atoi(matrixno)][0][1])];
                                                for (i = 1; i <= matrises[atoi(matrixno)][0][1]; i++)
                                                {
                                                    if ((matrises[atoi(matrixno)][a][i]) < val)
                                                    {
                                                        val = matrises[atoi(matrixno)][a][i];
                                                    }
                                                    matrises[atoi(matrixno)][a][b] = val;
                                                }
                                            }
                                        }
                                        for (a = (matrises[atoi(matrixno)][0][0]) + 1; a <= (matrises[atoi(matrixno)][0][0]) + atoi(row); a++)
                                        {
                                            for (b = 1; b <= (matrises[atoi(matrixno)][0][1]) + atoi(column); b++)
                                            {
                                                val = matrises[atoi(matrixno)][a - (matrises[atoi(matrixno)][0][0])][b];
                                                for (i = 1; i <= matrises[atoi(matrixno)][0][0]; i++)
                                                {
                                                    if ((matrises[atoi(matrixno)][i][b]) < val)
                                                    {
                                                        val = matrises[atoi(matrixno)][i][b];
                                                    }
                                                    matrises[atoi(matrixno)][a][b] = val;
                                                }
                                            }
                                        }
                                        matrises[atoi(matrixno)][0][0] = (matrises[atoi(matrixno)][0][0]) + atoi(row);
                                        matrises[atoi(matrixno)][0][1] = (matrises[atoi(matrixno)][0][1]) + atoi(column);
                                        fprintf(output, "matrix paded %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                        for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                        {
                                            for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                            {
                                                if (b < matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                                }
                                                else if (b == matrises[atoi(matrixno)][0][1])
                                                {
                                                    fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                                }
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
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(row, 0, 5 * sizeof(char));
                        free(row);
                        memset(column, 0, 5 * sizeof(char));
                        free(column);
                        memset(mode, 0, 10 * sizeof(char));
                        free(mode);
                        memset(readen, 0, 10 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF PADVAL
                    else if (!strcmp(command, "padval"))
                    {
                        char *matrix = calloc(5, sizeof(char));
                        char *row = calloc(5, sizeof(char));
                        char *column = calloc(5, sizeof(char));
                        char *val = calloc(5, sizeof(char));
                        char *point = line;
                        int nchars, word = 0;
                        char *readen = calloc(10, sizeof(char));
                        word++;
                        while (*point)
                        {
                            memset(readen, 0, 10 * sizeof(char));
                            for (; *point; point++)
                            {
                                if ((*point >= 'a' && *point <= 'z') || (*point >= '0' && *point <= '9') || (*point == '-'))
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
                                        matrix[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        row[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        column[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        val[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrixno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        if (row[0] == '-')
                        {
                            for (i = 1; i <= 4; i++)
                            {
                                row[i - 1] = row[i];
                            }
                        }
                        if (column[0] == '-')
                        {
                            for (i = 1; i <= 4; i++)
                            {
                                column[i - 1] = column[i];
                            }
                        }
                        if (matrix[0] == 'm')
                        {
                            if (matrises[0][0][atoi(matrixno)] != 0)
                            {
                                if (atoi(row) >= 0 && atoi(column) >= 0)
                                {
                                    matrises[atoi(matrixno)] = realloc(matrises[atoi(matrixno)], ((matrises[atoi(matrixno)][0][0]) + atoi(row) + 1) * sizeof(int *));
                                    for (i = 1; i <= matrises[atoi(matrixno)][0][0]; i++)
                                    {
                                        matrises[atoi(matrixno)][i] = realloc(matrises[atoi(matrixno)][i], ((matrises[atoi(matrixno)][0][1]) + atoi(column) + 1) * sizeof(int));
                                    }
                                    int a, b;
                                    for (a = (matrises[atoi(matrixno)][0][0]) + 1; a <= (matrises[atoi(matrixno)][0][0]) + atoi(row); a++)
                                    {
                                        matrises[atoi(matrixno)][a] = calloc((matrises[atoi(matrixno)][0][1]) + atoi(column) + 1, sizeof(int));
                                    }
                                    for (a = 1; a <= (matrises[atoi(matrixno)][0][0]); a++)
                                    {
                                        for (b = (matrises[atoi(matrixno)][0][1]) + 1; b <= (matrises[atoi(matrixno)][0][1]) + atoi(column); b++)
                                        {
                                            matrises[atoi(matrixno)][a][b] = atoi(val);
                                        }
                                    }
                                    for (a = (matrises[atoi(matrixno)][0][0]) + 1; a <= (matrises[atoi(matrixno)][0][0]) + atoi(row); a++)
                                    {
                                        for (b = 1; b <= (matrises[atoi(matrixno)][0][1]) + atoi(column); b++)
                                        {
                                            matrises[atoi(matrixno)][a][b] = atoi(val);
                                        }
                                    }
                                    matrises[atoi(matrixno)][0][0] = (matrises[atoi(matrixno)][0][0]) + atoi(row);
                                    matrises[atoi(matrixno)][0][1] = (matrises[atoi(matrixno)][0][1]) + atoi(column);
                                    fprintf(output, "matrix paded %s %d %d\n", matrix, matrises[atoi(matrixno)][0][0], matrises[atoi(matrixno)][0][1]);
                                    for (a = 1; a <= matrises[atoi(matrixno)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrixno)][0][1]; b++)
                                        {
                                            if (b < matrises[atoi(matrixno)][0][1])
                                            {
                                                fprintf(output, "%d ", matrises[atoi(matrixno)][a][b]);
                                            }
                                            else if (b == matrises[atoi(matrixno)][0][1])
                                            {
                                                fprintf(output, "%d\n", matrises[atoi(matrixno)][a][b]);
                                            }
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
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(row, 0, 5 * sizeof(char));
                        free(row);
                        memset(column, 0, 5 * sizeof(char));
                        free(column);
                        memset(val, 0, 5 * sizeof(char));
                        free(val);
                        memset(readen, 0, 10 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF MATSLICEROW
                    else if (!strcmp(command, "matslicerow"))
                    {
                        char *matrix = calloc(5, sizeof(char));
                        char *index = calloc(5, sizeof(char));
                        char *start = calloc(5, sizeof(char));
                        char *stop = calloc(5, sizeof(char));
                        char *name = calloc(5, sizeof(char));
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
                                        matrix[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        index[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        start[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        stop[i] = readen[i];
                                    }
                                }
                                if (word == 6)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        name[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrixno = calloc(4, sizeof(char));
                        char *nameno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            nameno[i - 1] = name[i];
                        }
                        if (matrix[0] == 'm' && name[0] == 'v')
                        {
                            if (vectors[0][atoi(nameno)] == 0 && matrises[0][0][atoi(matrixno)] != 0)
                            {
                                if (1 <= atoi(index) && atoi(index) <= matrises[atoi(matrixno)][0][0] && 0 <= atoi(start) && atoi(start) < matrises[atoi(matrixno)][0][1] && 0 <= atoi(stop) && atoi(stop) <= matrises[atoi(matrixno)][0][1])
                                {
                                    if (atoi(start) != atoi(stop))
                                    {
                                        if (atoi(stop) > atoi(start))
                                        {
                                            vectors[atoi(nameno)] = calloc(atoi(stop) - atoi(start) + 1, sizeof(int));
                                            vectors[0][atoi(nameno)] = atoi(nameno);
                                            vectors[atoi(nameno)][0] = atoi(stop) - atoi(start);
                                            for (i = atoi(start) + 1; i < atoi(stop) + 1; i++)
                                            {
                                                vectors[atoi(nameno)][i - atoi(start)] = matrises[atoi(matrixno)][atoi(index) + 1][i];
                                            }
                                            fprintf(output, "vector sliced %s %d\n", name, vectors[atoi(nameno)][0]);
                                            for (i = 1; i <= vectors[atoi(nameno)][0]; i++)
                                            {
                                                if (i < vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d ", vectors[atoi(nameno)][i]);
                                                }
                                                else if (i == vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d\n", vectors[atoi(nameno)][i]);
                                                }
                                            }
                                        }
                                        else if (atoi(stop) < atoi(start))
                                        {
                                            vectors[atoi(nameno)] = calloc(atoi(start) - atoi(stop) + 1, sizeof(int));
                                            vectors[0][atoi(nameno)] = atoi(nameno);
                                            vectors[atoi(nameno)][0] = atoi(start) - atoi(stop);
                                            for (i = atoi(start) + 1; i > atoi(stop) + 1; i--)
                                            {
                                                vectors[atoi(nameno)][i - atoi(start)] = matrises[atoi(matrixno)][atoi(index) + 1][i];
                                            }
                                            fprintf(output, "vector sliced %s %d\n", name, vectors[atoi(nameno)][0]);
                                            for (i = 1; i <= vectors[atoi(nameno)][0]; i++)
                                            {
                                                if (i < vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d ", vectors[atoi(nameno)][i]);
                                                }
                                                else if (i == vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d\n", vectors[atoi(nameno)][i]);
                                                }
                                            }
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
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(name, 0, 5 * sizeof(char));
                        free(name);
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(nameno, 0, 4 * sizeof(char));
                        free(nameno);
                        memset(index, 0, 5 * sizeof(char));
                        free(index);
                        memset(start, 0, 5 * sizeof(char));
                        free(start);
                        memset(stop, 0, 5 * sizeof(char));
                        free(stop);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF MATSLICECOL
                    else if (!strcmp(command, "matslicecol"))
                    {
                        char *matrix = calloc(5, sizeof(char));
                        char *index = calloc(5, sizeof(char));
                        char *start = calloc(5, sizeof(char));
                        char *stop = calloc(5, sizeof(char));
                        char *name = calloc(5, sizeof(char));
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
                                        matrix[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        index[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        start[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        stop[i] = readen[i];
                                    }
                                }
                                if (word == 6)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        name[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrixno = calloc(4, sizeof(char));
                        char *nameno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            nameno[i - 1] = name[i];
                        }
                        if (matrix[0] == 'm' && name[0] == 'v')
                        {
                            if (vectors[0][atoi(nameno)] == 0 && matrises[0][0][atoi(matrixno)] != 0)
                            {
                                if (1 <= atoi(index) <= matrises[atoi(matrixno)][0][1] && 0 <= atoi(start) < matrises[atoi(matrixno)][0][0] && 0 <= atoi(stop) <= matrises[atoi(matrixno)][0][0])
                                {
                                    if (atoi(start) != atoi(stop))
                                    {
                                        if (atoi(stop) > atoi(start))
                                        {
                                            vectors[atoi(nameno)] = calloc(atoi(stop) - atoi(start) + 1, sizeof(int));
                                            vectors[0][atoi(nameno)] = atoi(nameno);
                                            vectors[atoi(nameno)][0] = atoi(stop) - atoi(start);
                                            for (i = atoi(start) + 1; i < atoi(stop) + 1; i++)
                                            {
                                                vectors[atoi(nameno)][i - atoi(start)] = matrises[atoi(matrixno)][i][atoi(index) + 1];
                                            }
                                            fprintf(output, "vector sliced %s %d\n", name, vectors[atoi(nameno)][0]);
                                            for (i = 1; i <= vectors[atoi(nameno)][0]; i++)
                                            {
                                                if (i < vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d ", vectors[atoi(nameno)][i]);
                                                }
                                                else if (i == vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d\n", vectors[atoi(nameno)][i]);
                                                }
                                            }
                                        }
                                        else if (atoi(stop) < atoi(start))
                                        {
                                            vectors[atoi(nameno)] = calloc(atoi(start) - atoi(stop) + 1, sizeof(int));
                                            vectors[0][atoi(nameno)] = atoi(nameno);
                                            vectors[atoi(nameno)][0] = atoi(start) - atoi(stop);
                                            for (i = atoi(start) + 1; i > atoi(stop) + 1; i--)
                                            {
                                                vectors[atoi(nameno)][i - atoi(start)] = matrises[atoi(matrixno)][i][atoi(index) + 1];
                                            }
                                            fprintf(output, "vector sliced %s %d\n", name, vectors[atoi(nameno)][0]);
                                            for (i = 1; i <= vectors[atoi(nameno)][0]; i++)
                                            {
                                                if (i < vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d ", vectors[atoi(nameno)][i]);
                                                }
                                                else if (i == vectors[atoi(nameno)][0])
                                                {
                                                    fprintf(output, "%d\n", vectors[atoi(nameno)][i]);
                                                }
                                            }
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
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(name, 0, 5 * sizeof(char));
                        free(name);
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(nameno, 0, 4 * sizeof(char));
                        free(nameno);
                        memset(index, 0, 5 * sizeof(char));
                        free(index);
                        memset(start, 0, 5 * sizeof(char));
                        free(start);
                        memset(stop, 0, 5 * sizeof(char));
                        free(stop);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF MATSLICE
                    else if (!strcmp(command, "matslice"))
                    { // not much to say at this point they almost look same.
                        char *matrix = calloc(5, sizeof(char));
                        char *row1 = calloc(5, sizeof(char));
                        char *row2 = calloc(5, sizeof(char));
                        char *col1 = calloc(5, sizeof(char));
                        char *col2 = calloc(5, sizeof(char));
                        char *name = calloc(5, sizeof(char));
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
                                        matrix[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        col1[i] = readen[i];
                                    }
                                }
                                if (word == 4)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        col2[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        row1[i] = readen[i];
                                    }
                                }
                                if (word == 6)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        row2[i] = readen[i];
                                    }
                                }
                                if (word == 7)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        name[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrixno = calloc(4, sizeof(char));
                        char *nameno = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrixno[i - 1] = matrix[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            nameno[i - 1] = name[i];
                        }
                        if (matrix[0] == 'm' && name[0] == 'm')
                        {
                            if (matrises[0][0][atoi(nameno)] == 0 && matrises[0][0][atoi(matrixno)] != 0)
                            {
                                if (1 <= atoi(row1) <= matrises[atoi(matrixno)][0][0] && 1 <= atoi(row2) <= matrises[atoi(matrixno)][0][0] &&
                                    1 <= atoi(col1) <= matrises[atoi(matrixno)][0][1] && 1 <= atoi(col2) <= matrises[atoi(matrixno)][0][1])
                                {
                                    if (atoi(row2) >= atoi(row1))
                                    {
                                        if (atoi(col2) >= atoi(col1))
                                        {
                                            matrises[0][0][atoi(nameno)] = atoi(nameno);
                                            matrises[atoi(nameno)] = calloc(atoi(row2) - atoi(row1) + 1, sizeof(int *));
                                            matrises[atoi(nameno)][0] = calloc(2, sizeof(int));
                                            matrises[atoi(nameno)][0][0] = atoi(row2) - atoi(row1);
                                            matrises[atoi(nameno)][0][1] = atoi(col2) - atoi(col1);
                                            int a, b;
                                            for (a = 1; a <= atoi(row2) - atoi(row1); a++)
                                            {
                                                matrises[atoi(nameno)][a] = calloc(atoi(col2) - atoi(col1) + 1, sizeof(int));
                                                for (b = 1; b <= atoi(col2) - atoi(col1); b++)
                                                {
                                                    matrises[atoi(nameno)][a][b] = matrises[atoi(matrixno)][a + atoi(row1)][b + atoi(col1)];
                                                }
                                            }
                                            fprintf(output, "matrix sliced %s %d %d\n", name, matrises[atoi(nameno)][0][0], matrises[atoi(nameno)][0][1]);
                                            for (a = 1; a <= matrises[atoi(nameno)][0][0]; a++)
                                            {
                                                for (b = 1; b <= matrises[atoi(nameno)][0][1]; b++)
                                                {
                                                    if (b < matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d ", matrises[atoi(nameno)][a][b]);
                                                    }
                                                    else if (b == matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d\n", matrises[atoi(nameno)][a][b]);
                                                    }
                                                }
                                            }
                                        }
                                        else if (atoi(col2) < atoi(col1))
                                        {
                                            matrises[0][0][atoi(nameno)] = atoi(nameno);
                                            matrises[atoi(nameno)] = calloc(atoi(row2) - atoi(row1) + 1, sizeof(int *));
                                            matrises[atoi(nameno)][0] = calloc(2, sizeof(int));
                                            matrises[atoi(nameno)][0][0] = atoi(row2) - atoi(row1);
                                            matrises[atoi(nameno)][0][1] = atoi(col1) - atoi(col2);
                                            int a, b;
                                            for (a = 1; a <= atoi(row2) - atoi(row1); a++)
                                            {
                                                matrises[atoi(nameno)][a] = calloc(atoi(col1) - atoi(col2) + 1, sizeof(int));
                                                for (b = 1; b <= atoi(col1) - atoi(col2); b++)
                                                {
                                                    matrises[atoi(nameno)][a][b] = matrises[atoi(matrixno)][a + atoi(row1)][b + atoi(col2)];
                                                }
                                            }
                                            fprintf(output, "matrix sliced %s %d %d\n", name, matrises[atoi(nameno)][0][0], matrises[atoi(nameno)][0][1]);
                                            for (a = 1; a <= matrises[atoi(nameno)][0][0]; a++)
                                            {
                                                for (b = 1; b <= matrises[atoi(nameno)][0][1]; b++)
                                                {
                                                    if (b < matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d ", matrises[atoi(nameno)][a][b]);
                                                    }
                                                    else if (b == matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d\n", matrises[atoi(nameno)][a][b]);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else if (atoi(row2) < atoi(row1))
                                    {
                                        if (atoi(col2) >= atoi(col1))
                                        {
                                            matrises[0][0][atoi(nameno)] = atoi(nameno);
                                            matrises[atoi(nameno)] = calloc(atoi(row1) - atoi(row2) + 1, sizeof(int *));
                                            matrises[atoi(nameno)][0] = calloc(2, sizeof(int));
                                            matrises[atoi(nameno)][0][0] = atoi(row1) - atoi(row2);
                                            matrises[atoi(nameno)][0][1] = atoi(col2) - atoi(col1);
                                            int a, b;
                                            for (a = 1; a <= atoi(row1) - atoi(row2); a++)
                                            {
                                                matrises[atoi(nameno)][a] = calloc(atoi(col2) - atoi(col1) + 1, sizeof(int));
                                                for (b = 1; b <= atoi(col2) - atoi(col1); b++)
                                                {
                                                    matrises[atoi(nameno)][a][b] = matrises[atoi(matrixno)][a + atoi(row2)][b + atoi(col1)];
                                                }
                                            }
                                            fprintf(output, "matrix sliced %s %d %d\n", name, matrises[atoi(nameno)][0][0], matrises[atoi(nameno)][0][1]);
                                            for (a = 1; a <= matrises[atoi(nameno)][0][0]; a++)
                                            {
                                                for (b = 1; b <= matrises[atoi(nameno)][0][1]; b++)
                                                {
                                                    if (b < matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d ", matrises[atoi(nameno)][a][b]);
                                                    }
                                                    else if (b == matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d\n", matrises[atoi(nameno)][a][b]);
                                                    }
                                                }
                                            }
                                        }
                                        else if (atoi(col2) < atoi(col1))
                                        {
                                            matrises[0][0][atoi(nameno)] = atoi(nameno);
                                            matrises[atoi(nameno)] = calloc(atoi(row1) - atoi(row2) + 1, sizeof(int *));
                                            matrises[atoi(nameno)][0] = calloc(2, sizeof(int));
                                            matrises[atoi(nameno)][0][0] = atoi(row1) - atoi(row2);
                                            matrises[atoi(nameno)][0][1] = atoi(col1) - atoi(col2);
                                            int a, b;
                                            for (a = 1; a <= atoi(row1) - atoi(row2); a++)
                                            {
                                                matrises[atoi(nameno)][a] = calloc(atoi(col1) - atoi(col2) + 1, sizeof(int));
                                                for (b = 1; b <= atoi(col1) - atoi(col2); b++)
                                                {
                                                    matrises[atoi(nameno)][a][b] = matrises[atoi(matrixno)][a + atoi(row2)][b + atoi(col2)];
                                                }
                                            }
                                            fprintf(output, "matrix sliced %s %d %d\n", name, matrises[atoi(nameno)][0][0], matrises[atoi(nameno)][0][1]);
                                            for (a = 1; a <= matrises[atoi(nameno)][0][0]; a++)
                                            {
                                                for (b = 1; b <= matrises[atoi(nameno)][0][1]; b++)
                                                {
                                                    if (b < matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d ", matrises[atoi(nameno)][a][b]);
                                                    }
                                                    else if (b == matrises[atoi(nameno)][0][1])
                                                    {
                                                        fprintf(output, "%d\n", matrises[atoi(nameno)][a][b]);
                                                    }
                                                }
                                            }
                                        }
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
                        memset(matrix, 0, 5 * sizeof(char));
                        free(matrix);
                        memset(name, 0, 5 * sizeof(char));
                        free(name);
                        memset(matrixno, 0, 4 * sizeof(char));
                        free(matrixno);
                        memset(nameno, 0, 4 * sizeof(char));
                        free(nameno);
                        memset(row1, 0, 5 * sizeof(char));
                        free(row1);
                        memset(row2, 0, 5 * sizeof(char));
                        free(row2);
                        memset(col1, 0, 5 * sizeof(char));
                        free(col1);
                        memset(col2, 0, 5 * sizeof(char));
                        free(col2);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF ADD
                    else if (!strcmp(command, "add"))
                    {
                        char *matrix1 = calloc(5, sizeof(char));
                        char *matrix2 = calloc(5, sizeof(char));
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
                                        matrix1[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        matrix2[i] = readen[i];
                                    }
                                }
                                if (word == 5)
                                {
                                    break;
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrix1no = calloc(4, sizeof(char));
                        char *matrix2no = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrix1no[i - 1] = matrix1[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            matrix2no[i - 1] = matrix2[i];
                        }
                        if (matrix1[0] == 'm' && matrix2[0] == 'm' && word <= 5)
                        {
                            if (matrises[0][0][atoi(matrix1no)] != 0 && matrises[0][0][atoi(matrix2no)] != 0)
                            {
                                if (matrises[atoi(matrix1no)][0][0] == matrises[atoi(matrix2no)][0][0] && matrises[atoi(matrix1no)][0][1] == matrises[atoi(matrix2no)][0][1])
                                {
                                    int a, b;
                                    for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                        {
                                            matrises[atoi(matrix1no)][a][b] = (matrises[atoi(matrix1no)][a][b]) + (matrises[atoi(matrix2no)][a][b]);
                                        }
                                    }
                                    fprintf(output, "add %s %s\n", matrix1, matrix2);
                                    for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                        {
                                            if (b < matrises[atoi(matrix1no)][0][1])
                                            {
                                                fprintf(output, "%d ", matrises[atoi(matrix1no)][a][b]);
                                            }
                                            else if (b == matrises[atoi(matrix1no)][0][1])
                                            {
                                                fprintf(output, "%d\n", matrises[atoi(matrix1no)][a][b]);
                                            }
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
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(matrix1, 0, 5 * sizeof(char));
                        free(matrix1);
                        memset(matrix2, 0, 5 * sizeof(char));
                        free(matrix2);
                        memset(matrix1no, 0, 4 * sizeof(char));
                        free(matrix1no);
                        memset(matrix2no, 0, 4 * sizeof(char));
                        free(matrix2no);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF MULTIPLY
                    else if (!strcmp(command, "multiply"))
                    {
                        char *matrix1 = calloc(5, sizeof(char));
                        char *matrix2 = calloc(5, sizeof(char));
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
                                        matrix1[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        matrix2[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrix1no = calloc(4, sizeof(char));
                        char *matrix2no = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrix1no[i - 1] = matrix1[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            matrix2no[i - 1] = matrix2[i];
                        }
                        if (matrix1[0] == 'm' && matrix2[0] == 'm')
                        {
                            if (matrises[0][0][atoi(matrix1no)] != 0 && matrises[0][0][atoi(matrix2no)] != 0)
                            {
                                if (matrises[atoi(matrix1no)][0][0] == matrises[atoi(matrix2no)][0][0] && matrises[atoi(matrix1no)][0][1] == matrises[atoi(matrix2no)][0][1])
                                {
                                    int a, b;
                                    for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                        {
                                            matrises[atoi(matrix1no)][a][b] = (matrises[atoi(matrix1no)][a][b]) * (matrises[atoi(matrix2no)][a][b]);
                                        }
                                    }
                                    fprintf(output, "multiply %s %s\n", matrix1, matrix2);
                                    for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                        {
                                            if (b < matrises[atoi(matrix1no)][0][1])
                                            {
                                                fprintf(output, "%d ", matrises[atoi(matrix1no)][a][b]);
                                            }
                                            else if (b == matrises[atoi(matrix1no)][0][1])
                                            {
                                                fprintf(output, "%d\n", matrises[atoi(matrix1no)][a][b]);
                                            }
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
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(matrix1, 0, 5 * sizeof(char));
                        free(matrix1);
                        memset(matrix2, 0, 5 * sizeof(char));
                        free(matrix2);
                        memset(matrix1no, 0, 4 * sizeof(char));
                        free(matrix1no);
                        memset(matrix2no, 0, 4 * sizeof(char));
                        free(matrix2no);
                        memset(readen, 0, 15 * sizeof(char));
                        free(readen);
                    } // BEGINNING OF SUBTRACT
                    else if (!strcmp(command, "subtract"))
                    {
                        char *matrix1 = calloc(5, sizeof(char));
                        char *matrix2 = calloc(5, sizeof(char));
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
                                        matrix1[i] = readen[i];
                                    }
                                }
                                if (word == 3)
                                {
                                    for (i = 0; readen[i]; i++)
                                    {
                                        matrix2[i] = readen[i];
                                    }
                                }
                            }
                            point += nchars;
                            word++;
                        }
                        char *matrix1no = calloc(4, sizeof(char));
                        char *matrix2no = calloc(4, sizeof(char));
                        for (i = 1; i <= 4; i++)
                        {
                            matrix1no[i - 1] = matrix1[i];
                        }
                        for (i = 1; i <= 4; i++)
                        {
                            matrix2no[i - 1] = matrix2[i];
                        }
                        if (matrix1[0] == 'm' && matrix2[0] == 'm')
                        {
                            if (matrises[0][0][atoi(matrix1no)] != 0 && matrises[0][0][atoi(matrix2no)] != 0)
                            {
                                if (matrises[atoi(matrix1no)][0][0] == matrises[atoi(matrix2no)][0][0] && matrises[atoi(matrix1no)][0][1] == matrises[atoi(matrix2no)][0][1])
                                {
                                    int a, b;
                                    for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                        {
                                            matrises[atoi(matrix1no)][a][b] = (matrises[atoi(matrix1no)][a][b]) - (matrises[atoi(matrix2no)][a][b]);
                                        }
                                    }
                                    fprintf(output, "subtract %s %s\n", matrix1, matrix2);
                                    for (a = 1; a <= matrises[atoi(matrix1no)][0][0]; a++)
                                    {
                                        for (b = 1; b <= matrises[atoi(matrix1no)][0][1]; b++)
                                        {
                                            if (b < matrises[atoi(matrix1no)][0][1])
                                            {
                                                fprintf(output, "%d ", matrises[atoi(matrix1no)][a][b]);
                                            }
                                            else if (b == matrises[atoi(matrix1no)][0][1])
                                            {
                                                fprintf(output, "%d\n", matrises[atoi(matrix1no)][a][b]);
                                            }
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
                        }
                        else
                        {
                            fprintf(output, "error\n");
                        }
                        memset(matrix1, 0, 5 * sizeof(char));
                        free(matrix1);
                        memset(matrix2, 0, 5 * sizeof(char));
                        free(matrix2);
                        memset(matrix1no, 0, 4 * sizeof(char));
                        free(matrix1no);
                        memset(matrix2no, 0, 4 * sizeof(char));
                        free(matrix2no);
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
                memset(line, 0, 50 * sizeof(char));
                free(line);
            }
            fclose(output); // closing the output file, now the work is done hopefully there is no output errors
        }
        fclose(commands); // closing the input file.
    }
    else if (argc > 4)
    {
        printf("ERROR: Too many arguments supplied.\n");
    }
    else
    {
        printf("ERROR: Insufficent arguments.\n");
    }
    int no, numb, count, in;
    for (no = 0; no < 100; no++)
    { // freeing our matrises, now the matrix list key and matrix keys are so usefull, i know which matrises exist and their size, nothing else i needed to free them!
        if (matrises[0][0][no])
        {
            if (matrises[0][0][no] != 0)
            {
                numb = matrises[0][0][no];
                in = matrises[numb][0][0];
                for (count = 0; count <= in; count++)
                {
                    free(matrises[numb][count]); // freeing from deepest dimension
                }
                free(matrises[numb]);
            }
        }
    }
    free(matrises[0][0]); // now forgeting to free matrix list key,
    free(matrises[0]);
    free(matrises); // and our matrises is freed, very customized 3d array, very usefull, took me days to figure out
    for (no = 0; no < 100; no++)
    {
        if (vectors[0][no])
        {
            if (vectors[0][no] != 0)
            {
                numb = vectors[0][no];
                free(vectors[numb]); // freeing the vectors has same logic but rather simpler
            }
        }
    }
    free(vectors[0]);
    free(vectors); // with the vectors array gone there is nothing left in the memory.
    return 1;      // ending the main, thanks if you read the comments :)
}