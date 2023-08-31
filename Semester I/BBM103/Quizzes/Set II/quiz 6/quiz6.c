#include <stdio.h>
/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/
int main()
{
    int temp;
    int i, j;
    int listA[10] = {12, 29, 15, 8, 36, 6, 9, 2, 4, 7};
    printf("List A: [");
    for (i = 0; i < 9; i++)
    {
        printf("%d, ", listA[i]);
    }
    printf("%d", listA[9]);
    printf("]\n");
    int listB[10] = {39, 41, 1, 3, 27, 14, 5, 11, 90, 43};
    printf("List B: [");
    for (i = 0; i < 9; i++)
    {
        printf("%d, ", listB[i]);
    }
    printf("%d", listB[9]);
    printf("]\n");
    int sortedListA[10];
    for (i = 0; i < 10; i++)
    {
        sortedListA[i] = listA[i];
    }
    for (i = 0; i < 10; i++)
    {
        for (j = i + 1; j < 10; j++)
        {
            if (sortedListA[i] > sortedListA[j])
            {
                temp = sortedListA[i];
                sortedListA[i] = sortedListA[j];
                sortedListA[j] = temp;
            }
        }
    }
    printf("Sorted List A: [");
    for (i = 0; i < 9; i++)
    {
        printf("%d, ", sortedListA[i]);
    }
    printf("%d", sortedListA[9]);
    printf("]\n");
    int sortedListB[10];
    for (i = 0; i < 10; i++)
    {
        sortedListB[i] = listB[i];
    }
    for (i = 0; i < 10; i++)
    {
        for (j = i + 1; j < 10; j++)
        {
            if (sortedListB[i] > sortedListB[j])
            {
                temp = sortedListB[i];
                sortedListB[i] = sortedListB[j];
                sortedListB[j] = temp;
            }
        }
    }
    printf("Sorted List B: [");
    for (i = 0; i < 9; i++)
    {
        printf("%d, ", sortedListB[i]);
    }
    printf("%d", sortedListB[9]);
    printf("]\n");
    int listC[20];
    for (i = 0; i < 10; i++)
    {
        listC[i] = sortedListA[i];
    }
    for (i = 0; i < 10; i++)
    {
        listC[i + 10] = sortedListB[i];
    }
    printf("List C: [");
    for (i = 0; i < 19; i++)
    {
        printf("%d, ", listC[i]);
    }
    printf("%d", listC[19]);
    printf("]\n");
    int sortedListC[20];
    for (i = 0; i < 20; i++)
    {
        sortedListC[i] = listC[i];
    }
    for (i = 0; i < 20; i++)
    {
        for (j = i + 1; j < 20; j++)
        {
            if (sortedListC[i] > sortedListC[j])
            {
                temp = sortedListC[i];
                sortedListC[i] = sortedListC[j];
                sortedListC[j] = temp;
            }
        }
    }
    printf("Sorted List C: [");
    for (i = 0; i < 19; i++)
    {
        printf("%d, ", sortedListC[i]);
    }
    printf("%d", sortedListC[19]);
    printf("]\n");
    return 0;
}