#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/formatConvert.h"

int main(int _iArgc, char *_pszArgv[])
{
	// Check arguments.
	if (_iArgc < 2)
	{
		printf("Error: no input file specified.\n");
		return 1;
	}

	// Open the input file for reading.
	FILE *pfInputCode = fopen(_pszArgv[1], "r");
	if (pfInputCode == NULL)
	{
		printf("Error: unable to open input file.\n");
		return 1;
	}

	// Create the output file name by adding "_beautified" before the .c in the file name.
	char szOutputFileName[100];
	strcpy(szOutputFileName, _pszArgv[1]);
	char *pszDotPos = strrchr(szOutputFileName, '.');
	if (pszDotPos == NULL)
	{
		printf("Error: input file must have a .c extension.\n");
		return 1;
	}
	strcpy(pszDotPos, "_beautified.c");

	// Open the output file for writing
	FILE *pfOutputCode = fopen(szOutputFileName, "w");
	if (pfOutputCode == NULL)
	{
		printf("Error: unable to open output file.\n");
		return 1;
	}

	int iIndentIndex = 0;
	int iNumLoopVars = 0;
	char **ppszLoopVars = (char **)malloc(iNumLoopVars * sizeof(char *));

	char szCodeLine[CODE_LINE_SIZE];

	// Read the input file line by line and convert for loops to while loops.
	while (fgets(szCodeLine, CODE_LINE_SIZE, pfInputCode))
	{
		formatCode(&pfInputCode, &pfOutputCode, &iIndentIndex, 0, &iNumLoopVars, &ppszLoopVars, &szCodeLine);
	}

	// Clean up.
	fclose(pfInputCode);
	fclose(pfOutputCode);

	for (size_t i = 0; i < iNumLoopVars; i++)
	{
		free(ppszLoopVars[i]);
	}
	free(ppszLoopVars);

	return 0;
}
