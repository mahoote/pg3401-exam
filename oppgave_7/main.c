#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "include/formatConvert.h"

int main(int argc, char *argv[])
{
	// Check that a file was passed as an argument
	if (argc < 2)
	{
		printf("Error: no input file specified.\n");
		return 1;
	}

	// Open the input file for reading
	FILE *in_file = fopen(argv[1], "r");
	if (in_file == NULL)
	{
		printf("Error: unable to open input file.\n");
		return 1;
	}

	// Create the output file name by adding "_beautified" before the .c in the file name
	char out_file_name[100];
	strcpy(out_file_name, argv[1]);
	char *dot = strrchr(out_file_name, '.');
	if (dot == NULL)
	{
		printf("Error: input file must have a .c extension.\n");
		return 1;
	}
	strcpy(dot, "_beautified.c");

	// Open the output file for writing
	FILE *out_file = fopen(out_file_name, "w");
	if (out_file == NULL)
	{
		printf("Error: unable to open output file.\n");
		return 1;
	}

	int iIndentIndex = 0;
	int iNumLoopVars = 0;
	char **ppszLoopVars = (char **)malloc(iNumLoopVars * sizeof(char *));

	char szCodeLine[CODE_LINE_SIZE];

	// Read the input file line by line and convert for loops to while loops
	while (fgets(szCodeLine, sizeof(szCodeLine), in_file))
	{
		formatCode(&in_file, &out_file, &iIndentIndex, 0, &iNumLoopVars, &ppszLoopVars, &szCodeLine);
	}

	fclose(in_file);
	fclose(out_file);

	for (size_t i = 0; i < iNumLoopVars; i++)
	{
		free(ppszLoopVars[i]);
	}
	free(ppszLoopVars);

	return 0;
}
