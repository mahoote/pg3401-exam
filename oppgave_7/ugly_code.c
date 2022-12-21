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

    for (size_t i = 0; i < iWords; i++)
    {
        int iRandNumber = rand() % 5;
        pszWordList[i] = (char *)malloc(256 * sizeof(char));
        strcpy(pszWordList[i], pszWordExamples[iRandNumber]);
    }

    for (size_t i = 0; i < iWords; i++)
    {
        printf("The %d. word: %s\n", i, pszWordList[i]);
    }

    return 0;
}