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

	for (size_t i = 0; i < iWords; i++)
	{
		int iRandNumber = rand() % 5;
		ppszWordList[i] = (char *)malloc(256 * sizeof(char));
		strcpy(ppszWordList[i], pszWordExamples[iRandNumber]);
	}

	for (size_t i = 0; i < iWords; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			printf("Hello ");

			for (size_t o = 0; o < 1; o++)
			{
				printf("World\n");
			}
		}

		printf("The %d. word: %s\n", i, ppszWordList[i]);
	}

	return 0;
}
