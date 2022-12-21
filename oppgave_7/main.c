#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INDENT_SIZE 3

void addIndent(FILE **_ppfOutFile, int *_piIndentIndex, int _iIndentSize)
{
	for (size_t i = 0; i < *_piIndentIndex * _iIndentSize; i++)
	{
		fputc(' ', *_ppfOutFile);
	}
}

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

	// Read the input file line by line and convert for loops to while loops
	char szCodeLine[1024];
	while (fgets(szCodeLine, sizeof(szCodeLine), in_file))
	{
		// Check if the line is a for loop
		if (strstr(szCodeLine, "for (") != NULL)
		{
			iIndentIndex++;

			// Extract the loop variable, condition, and increment from the for loop
			char *start = strchr(szCodeLine, '(');
			char *end = strchr(szCodeLine, ')');
			if (start == NULL || end == NULL)
			{
				printf("Error: invalid for loop syntax.\n");
				return 1;
			}
			*end = '\0';
			char loop_var[100];
			char condition[100];
			char increment[100];
			sscanf(start + 1, "%[^;];%[^;];%s", loop_var, condition, increment);

			bool bLoopVarExists = false;
			for (size_t i = 0; i < iNumLoopVars; i++)
			{
				if (strcmp(ppszLoopVars[i], loop_var) == 0)
				{
					bLoopVarExists = true;
				}
			}

			// Checking if the variable has been declared. If so, do not redeclare it.
			if (!bLoopVarExists)
			{
				// Save loop var in list.
				iNumLoopVars++;
				ppszLoopVars = (char **)realloc(ppszLoopVars, (iNumLoopVars) * sizeof(ppszLoopVars));
				ppszLoopVars[iNumLoopVars - 1] = (char *)malloc(100 * sizeof(char));
				strcpy(ppszLoopVars[iNumLoopVars - 1], loop_var);
			}
			else
			{
				char *first_space = strchr(loop_var, ' ');

				// Check if a space was found
				if (!first_space)
				{
					printf("Error: invalid loop variable.\n");
				}

				memcpy(loop_var, first_space + 1, 100);
			}

			// Write the loop variable and condition to the output file
			addIndent(&out_file, &iIndentIndex, INDENT_SIZE);
			fprintf(out_file, "%s;\n", loop_var);

			addIndent(&out_file, &iIndentIndex, INDENT_SIZE);
			fprintf(out_file, "while (%s)\n", condition);

			iIndentIndex++;

			// Read the input file until the end of the for loop body
			while (fgets(szCodeLine, sizeof(szCodeLine), in_file) && strstr(szCodeLine, "}") == NULL)
			{
				// Replace tabs with 3 spaces and write the line to the output file
				for (int i = 0; i < strlen(szCodeLine); i++)
				{
					if (szCodeLine[i] == '\t')
					{
						fputc(' ', out_file);
						fputc(' ', out_file);
						fputc(' ', out_file);
					}
					else
					{
						fputc(szCodeLine[i], out_file);
					}
				}
			}
			// Write the increment to the output file
			addIndent(&out_file, &iIndentIndex, INDENT_SIZE);
			fprintf(out_file, "%s;\n", increment);

			iIndentIndex--;

			addIndent(&out_file, &iIndentIndex, INDENT_SIZE);
			fprintf(out_file, "}\n");

			iIndentIndex--;
		}
		else
		{
			// Replace tabs with 3 spaces and write the line to the output file
			for (int i = 0; i < strlen(szCodeLine); i++)
			{
				if (szCodeLine[i] == '\t')
				{
					fputc(' ', out_file);
					fputc(' ', out_file);
					fputc(' ', out_file);
				}
				else
				{
					fputc(szCodeLine[i], out_file);
				}
			}
		}
	}

	// Close the input and output files
	fclose(in_file);
	fclose(out_file);

	for (size_t i = 0; i < iNumLoopVars; i++)
	{
		free(ppszLoopVars[i]);
	}
	free(ppszLoopVars);

	return 0;
}
