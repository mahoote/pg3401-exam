#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{

    int iWords = 100;
    char *pszWordExamples[] = {"Hello", "World", "C", "Is", "Fun"};
    char **pszWordList = (char **)malloc(iWords * sizeof(char *));

    srand(time(NULL));

size_t i = 0;
while ( i < iWords)
    {
        int iRandNumber = rand() % 5;
        pszWordList[i] = (char *)malloc(256 * sizeof(char));
        strcpy(pszWordList[i], pszWordExamples[iRandNumber]);
i++;
}

size_t i = 0;
while ( i < iWords)
    {
        printf("The %d. word: %s\n", i, pszWordList[i]);
i++;
}

    return 0;
}