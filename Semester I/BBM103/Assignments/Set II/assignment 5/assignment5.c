
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/
int main(int argc, char *argv[])
{               /* main function taking arguments as input */
    FILE *file; /* there is main defining part for this function*/
    char input[31];
    char inputamino[11];
    char aminocurrent[4];
    char Fenilalanin1[4] = "UUU"; /* All amino acids defined by their name and codon */
    char Fenilalanin2[4] = "UUC";
    char Losin1[4] = "UUA";
    char Losin2[4] = "UUG";
    char Losin3[4] = "CUU";
    char Losin4[4] = "CUC";
    char Losin5[4] = "CUA";
    char Losin6[4] = "CUG";
    char Izosilin1[4] = "AUU";
    char Izosilin2[4] = "AUC";
    char Izosilin3[4] = "AUA";
    char Metionin[4] = "AUG";
    char Valin1[4] = "GUU";
    char Valin2[4] = "GUC";
    char Valin3[4] = "GUA";
    char Valin4[4] = "GUG";
    char Serin1[4] = "UCU";
    char Serin2[4] = "UCC";
    char Serin3[4] = "UCA";
    char Serin4[4] = "UCG";
    char Prolin1[4] = "CCU";
    char Prolin2[4] = "CCC";
    char Prolin3[4] = "CCA";
    char Prolin4[4] = "CCG";
    char Treonin1[4] = "ACU";
    char Treonin2[4] = "ACC";
    char Treonin3[4] = "ACA";
    char Treonin4[4] = "ACG";
    char Alanin1[4] = "GCU";
    char Alanin2[4] = "GCC";
    char Alanin3[4] = "GCA";
    char Alanin4[4] = "GCG";
    char Triozin1[4] = "UAU";
    char Triozin2[4] = "UAC";
    char StopCodon1[4] = "UAA";
    char StopCodon2[4] = "UAG";
    char Histidin1[4] = "CAU";
    char Histidin2[4] = "CAC";
    char Glutamin1[4] = "CAA";
    char Glutamin2[4] = "CAG";
    char Asparajin1[4] = "AAU";
    char Asparajin2[4] = "AAC";
    char Lizin1[4] = "AAA";
    char Lizin2[4] = "AAG";
    char Aspertik1[4] = "GAU";
    char Aspertik2[4] = "GAC";
    char Glutamic1[4] = "GAA";
    char Glutamic2[4] = "GAG";
    char Sistein1[4] = "UGU";
    char Sistein2[4] = "UGC";
    char StopCodon3[4] = "UGA";
    char Triptofan[4] = "UGG";
    char Arjinin1[4] = "CGU";
    char Arjinin2[4] = "CGC";
    char Arjinin3[4] = "CGA";
    char Arjinin4[4] = "CGG";
    char Serin5[4] = "AGU";
    char Serin6[4] = "AGC";
    char Arjinin5[4] = "AGA";
    char Arjinin6[4] = "AGG";
    char Glisin1[4] = "GGU";
    char Glisin2[4] = "GGC";
    char Glisin3[4] = "GGA";
    char Glisin4[4] = "GGG";
    char myProteinA[31] = "MVAEGTKRIX"; /* All known proteins defined by their name and order of amino acid*/
    char myProteinB[31] = "MGEAVRKTIX";
    char myProteinC[31] = "MFSYCLPQRX";
    char myProteinD[31] = "MFLVPTYDHX";
    char myProteinE[31] = "MFSYCLPKRX";
    int validp = 1;             /* Our main integer to determine if given input is protein or not, later on when we are checking statements if we find a error we will set this 0*/
    file = fopen(argv[1], "r"); /* Opening given argument as input file */
    if (file)
    {
        fseek(file, 0, SEEK_SET);  /* Getting our memory set to beginning of the file, this fixes some bugs */
        fread(input, 31, 1, file); /* Reading our file into a char we defined as input[31] */
    }
    int x;
    for (x = 0; x <= 29; x = x + 3)
    {                                        /* Starting from 0 to end of the protein we every 3 letter*/
        strncpy(aminocurrent, input + x, 3); /* copy them to a current variable which gets clean every loop*/
        aminocurrent[3] = '\0';
        if (x == 0)
        { /* Checking if we are reading start codon */
            if (strcmp(aminocurrent, Metionin) == 0)
            { /* This part is unnecessary because later on we check start codon again but I am lazy to delete.*/
                strcat(inputamino, "M");
            }
            else
            {
                strcat(inputamino, "B");
                validp = 0;
            }
        }
        else
        { /* Reading rest of the protein and seeking for matches between given amino acids. Stop codon is X, Invalid codon is B.*/

            if (strcmp(aminocurrent, Fenilalanin1) == 0)
            {
                strcat(inputamino, "F");
            }
            else if (strcmp(aminocurrent, Fenilalanin2) == 0)
            {
                strcat(inputamino, "F");
            }
            else if (strcmp(aminocurrent, Losin1) == 0)
            {
                strcat(inputamino, "L");
            }
            else if (strcmp(aminocurrent, Losin2) == 0)
            {
                strcat(inputamino, "L");
            }
            else if (strcmp(aminocurrent, Losin3) == 0)
            {
                strcat(inputamino, "L");
            }
            else if (strcmp(aminocurrent, Losin4) == 0)
            {
                strcat(inputamino, "L");
            }
            else if (strcmp(aminocurrent, Losin5) == 0)
            {
                strcat(inputamino, "L");
            }
            else if (strcmp(aminocurrent, Losin6) == 0)
            {
                strcat(inputamino, "L");
            }
            else if (strcmp(aminocurrent, Izosilin1) == 0)
            {
                strcat(inputamino, "I");
            }
            else if (strcmp(aminocurrent, Izosilin2) == 0)
            {
                strcat(inputamino, "I");
            }
            else if (strcmp(aminocurrent, Izosilin3) == 0)
            {
                strcat(inputamino, "I");
            }
            else if (strcmp(aminocurrent, Metionin) == 0)
            {
                strcat(inputamino, "M");
            }
            else if (strcmp(aminocurrent, Valin1) == 0)
            {
                strcat(inputamino, "V");
            }
            else if (strcmp(aminocurrent, Valin2) == 0)
            {
                strcat(inputamino, "V");
            }
            else if (strcmp(aminocurrent, Valin3) == 0)
            {
                strcat(inputamino, "V");
            }
            else if (strcmp(aminocurrent, Valin4) == 0)
            {
                strcat(inputamino, "V");
            }
            else if (strcmp(aminocurrent, Serin1) == 0)
            {
                strcat(inputamino, "S");
            }
            else if (strcmp(aminocurrent, Serin2) == 0)
            {
                strcat(inputamino, "S");
            }
            else if (strcmp(aminocurrent, Serin3) == 0)
            {
                strcat(inputamino, "S");
            }
            else if (strcmp(aminocurrent, Serin4) == 0)
            {
                strcat(inputamino, "S");
            }
            else if (strcmp(aminocurrent, Prolin1) == 0)
            {
                strcat(inputamino, "P");
            }
            else if (strcmp(aminocurrent, Prolin2) == 0)
            {
                strcat(inputamino, "P");
            }
            else if (strcmp(aminocurrent, Prolin3) == 0)
            {
                strcat(inputamino, "P");
            }
            else if (strcmp(aminocurrent, Prolin4) == 0)
            {
                strcat(inputamino, "P");
            }
            else if (strcmp(aminocurrent, Treonin1) == 0)
            {
                strcat(inputamino, "T");
            }
            else if (strcmp(aminocurrent, Treonin2) == 0)
            {
                strcat(inputamino, "T");
            }
            else if (strcmp(aminocurrent, Treonin3) == 0)
            {
                strcat(inputamino, "T");
            }
            else if (strcmp(aminocurrent, Treonin4) == 0)
            {
                strcat(inputamino, "T");
            }
            else if (strcmp(aminocurrent, Alanin1) == 0)
            {
                strcat(inputamino, "A");
            }
            else if (strcmp(aminocurrent, Alanin2) == 0)
            {
                strcat(inputamino, "A");
            }
            else if (strcmp(aminocurrent, Alanin3) == 0)
            {
                strcat(inputamino, "A");
            }
            else if (strcmp(aminocurrent, Alanin4) == 0)
            {
                strcat(inputamino, "A");
            }
            else if (strcmp(aminocurrent, Triozin1) == 0)
            {
                strcat(inputamino, "Y");
            }
            else if (strcmp(aminocurrent, Triozin2) == 0)
            {
                strcat(inputamino, "Y");
            }
            else if (strcmp(aminocurrent, StopCodon1) == 0)
            {
                strcat(inputamino, "X");
            }
            else if (strcmp(aminocurrent, StopCodon2) == 0)
            {
                strcat(inputamino, "X");
            }
            else if (strcmp(aminocurrent, Histidin1) == 0)
            {
                strcat(inputamino, "H");
            }
            else if (strcmp(aminocurrent, Histidin2) == 0)
            {
                strcat(inputamino, "H");
            }
            else if (strcmp(aminocurrent, Glutamin1) == 0)
            {
                strcat(inputamino, "Q");
            }
            else if (strcmp(aminocurrent, Glutamin2) == 0)
            {
                strcat(inputamino, "Q");
            }
            else if (strcmp(aminocurrent, Asparajin1) == 0)
            {
                strcat(inputamino, "N");
            }
            else if (strcmp(aminocurrent, Asparajin2) == 0)
            {
                strcat(inputamino, "N");
            }
            else if (strcmp(aminocurrent, Lizin1) == 0)
            {
                strcat(inputamino, "K");
            }
            else if (strcmp(aminocurrent, Lizin2) == 0)
            {
                strcat(inputamino, "K");
            }
            else if (strcmp(aminocurrent, Aspertik1) == 0)
            {
                strcat(inputamino, "D");
            }
            else if (strcmp(aminocurrent, Aspertik2) == 0)
            {
                strcat(inputamino, "D");
            }
            else if (strcmp(aminocurrent, Glutamic1) == 0)
            {
                strcat(inputamino, "E");
            }
            else if (strcmp(aminocurrent, Glutamic2) == 0)
            {
                strcat(inputamino, "E");
            }
            else if (strcmp(aminocurrent, Sistein1) == 0)
            {
                strcat(inputamino, "C");
            }
            else if (strcmp(aminocurrent, Sistein2) == 0)
            {
                strcat(inputamino, "C");
            }
            else if (strcmp(aminocurrent, StopCodon3) == 0)
            {
                strcat(inputamino, "X");
            }
            else if (strcmp(aminocurrent, Triptofan) == 0)
            {
                strcat(inputamino, "W");
            }
            else if (strcmp(aminocurrent, Arjinin1) == 0)
            {
                strcat(inputamino, "R");
            }
            else if (strcmp(aminocurrent, Arjinin2) == 0)
            {
                strcat(inputamino, "R");
            }
            else if (strcmp(aminocurrent, Arjinin3) == 0)
            {
                strcat(inputamino, "R");
            }
            else if (strcmp(aminocurrent, Arjinin4) == 0)
            {
                strcat(inputamino, "R");
            }
            else if (strcmp(aminocurrent, Serin5) == 0)
            {
                strcat(inputamino, "S");
            }
            else if (strcmp(aminocurrent, Serin6) == 0)
            {
                strcat(inputamino, "S");
            }
            else if (strcmp(aminocurrent, Arjinin5) == 0)
            {
                strcat(inputamino, "R");
            }
            else if (strcmp(aminocurrent, Arjinin6) == 0)
            {
                strcat(inputamino, "R");
            }
            else if (strcmp(aminocurrent, Glisin1) == 0)
            {
                strcat(inputamino, "G");
            }
            else if (strcmp(aminocurrent, Glisin2) == 0)
            {
                strcat(inputamino, "G");
            }
            else if (strcmp(aminocurrent, Glisin3) == 0)
            {
                strcat(inputamino, "G");
            }
            else if (strcmp(aminocurrent, Glisin4) == 0)
            {
                strcat(inputamino, "G");
            }
            else
            { /* If given codon is not an aminoacid we set protein valid variable to 0*/
                strcat(inputamino, "B");
                validp = 0;
            }
        }
        strcpy(aminocurrent, ""); /* Clearing our current*/
    }
    char stopcodon[2];      /*Now we have 3 part check for if our input is a valid protein. */
    int stopcodonvalid = 0; /*  First we check if there is a stop codon.*/
    int b;
    for (b = 0; b < 10; b = b + 1)
    {
        strncpy(stopcodon, inputamino + b, 1);
        stopcodon[1] = '\0';
        if (strcmp(stopcodon, "X") == 0)
        {
            stopcodonvalid = 1;
        }
        strcpy(stopcodon, "");
    }
    if (stopcodonvalid == 0)
    { /* If there is not a stop codon we set protein valid to 0 */
        validp = 0;
    }
    char startcodon[2]; /* We check if start codon is AUG*/
    int startcodonvalid = 1;
    strncpy(startcodon, inputamino, 1);
    if (strcmp(startcodon, "M") != 0)
    {
        startcodonvalid = 0;
        validp = 0; /* If not we set protein variable to 0 */
    }
    char validcodon[2];
    int validcodonvalid = 1;
    int d;
    for (d = 0; d < 10; d = d + 1)
    { /* We check if there is a B which is undefined codon in given input*/
        strncpy(validcodon, inputamino + b, 1);
        validcodon[1] = '\0';
        if (strcmp(stopcodon, "B") == 0)
        {
            validcodonvalid = 0;
        }
        strcpy(validcodon, "");
    }
    if (validcodonvalid == 0)
    { /* If there is an undefined codon we set our protein valid to 0 */
        validp = 0;
    }

    if (strcmp(inputamino, myProteinA) == 0)
    {                                                                                                        /* Seeking for match with known proteins 1 by 1*/
        printf("MyProteinA is identified in sequence.\n\nThe amino acids of MyProteinA: M-V-A-E-G-T-K-R-I"); /* If match found giving requested print*/
    }
    else if (strcmp(inputamino, myProteinB) == 0)
    {
        printf("MyProteinB is identified in sequence.\n\nThe amino acids of MyProteinB: M-G-E-A-V-R-K-T-I");
    }
    else if (strcmp(inputamino, myProteinC) == 0)
    {
        printf("MyProteinC is identified in sequence.\n\nThe amino acids of MyProteinC: M-F-S-Y-C-L-P-Q-R");
    }
    else if (strcmp(inputamino, myProteinD) == 0)
    {
        printf("MyProteinD is identified in sequence.\n\nThe amino acids of MyProteinD: M-F-L-V-P-T-Y-D-H");
    }
    else if (strcmp(inputamino, myProteinE) == 0)
    {
        printf("MyProteinE is identified in sequence.\n\nThe amino acids of MyProteinE: M-F-S-Y-C-L-P-K-R");
    }
    else
    {
        printf("It is not a known protein.\n"); /* If match not found it is not a known protein*/
        if (validp == 1)
        { /* Checking if it is a valid protein */
            printf("It is probably a new protein.  ");
            int a;
            char currentletter[2];
            char nextletter[2];
            for (a = 0; a < 9; a = a + 1)
            { /* Printing the given input in amino acid form */
                strncpy(currentletter, inputamino + a, 1);
                currentletter[1] = '\0';
                strncpy(nextletter, inputamino + a + 1, 1);
                nextletter[1] = '\0';
                printf("%s", currentletter);
                if (strcmp(nextletter, "X") == 0)
                {
                    break;
                }
                else
                {
                    printf("-");
                }
                strcpy(currentletter, "");
            }
        }
        else
        {
            printf("It is not a protein."); /* If it is not a protein printing that*/
            if (stopcodonvalid == 0)
            {
                printf(" No stop code."); /* Printing specified errors. If there is more than 1 they will print. */
            }
            if (startcodonvalid == 0)
            {
                printf(" Not start with AUG.");
            }
            if (validcodonvalid == 0)
            {
                printf(" There are invalid amino acids.");
            }
        }
    }
    printf("\n"); /* Printing an \n at the end*/
    fclose(file); /* Closing our file */
    return (0);   /* Returning from main function */
}