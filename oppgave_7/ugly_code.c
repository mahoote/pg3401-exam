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

	if(1 == 1) 
	{
		int i = 0;

		for (i = 0; i < 5; i++) {
			printf("%d. line\n");
		}
		
	}


	for (size_t i = 0; i < iWords; i++) { 

		int iRandNumber = rand() % 5;
		ppszWordList[i] = (char *)malloc(256 * sizeof(char));
		strcpy(ppszWordList[i], pszWordExamples[iRandNumber]);

	}

	for (size_t i = 0; i < iWords; i++)
	{
		size_t a;
		for (a = 0; a < 3; a++)
		{
			printf("Hello ");

			for (size_t b = 0; b < 1; b++) {
				printf("World");
			}

			for (size_t b = 0; b < 1; b++) {
				printf("\n");
			}
		}

		for (a = 0; a < 3; a++)
		{
			// Remember to add awesome code!
		}

		printf("Word %d: %s", i, ppszWordList[i]);
	}
	

	return 0;
}
