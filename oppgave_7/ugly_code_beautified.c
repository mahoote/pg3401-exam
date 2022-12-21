#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{

   int iWords = 100;
   char *pszWordExamples[] = {"Hello", "World", "C", "Is", "Fun"};
   char **ppszWordList = (char **)malloc(iWords * sizeof(char *));

   srand(time(NULL));

   size_t i = 0;
   while ( i < iWords)
   {
      int iRandNumber = rand() % 5;
      ppszWordList[i] = (char *)malloc(256 * sizeof(char));
      strcpy(ppszWordList[i], pszWordExamples[iRandNumber]);
   i++;
   }

   size_t a = 0;
   while ( a < iWords)
   {
      printf("The %d. word: %s\n", a, ppszWordList[a]);
   a++;
   }

   return 0;
}