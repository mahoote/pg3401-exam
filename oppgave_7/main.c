#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void addIndent(FILE **out_file, int *iIndentIndex)
{
	for (size_t i = 0; i < *iIndentIndex * 3; i++)
	{
		fputc(' ', *out_file);
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
	int iLoopVars = 0;
	char **ppszLoopVars = (char **)malloc(iLoopVars * sizeof(char *));

	// Read the input file line by line and convert for loops to while loops
	char line[1000];
	while (fgets(line, sizeof(line), in_file))
	{
		// Check if the line is a for loop
		if (strstr(line, "for (") != NULL)
		{
			iIndentIndex++;

			// Extract the loop variable, condition, and increment from the for loop
			char *start = strchr(line, '(');
			char *end = strchr(line, ')');
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
			for (size_t i = 0; i < iLoopVars; i++)
			{
				if (strcmp(ppszLoopVars[i], loop_var) == 0)
				{
					bLoopVarExists = true;
				}
			}

			if (!bLoopVarExists)
			{
				// Save loop var in list.
				iLoopVars++;
				ppszLoopVars = (char **)realloc(ppszLoopVars, (iLoopVars) * sizeof(ppszLoopVars));
				ppszLoopVars[iLoopVars - 1] = (char *)malloc(100 * sizeof(char));
				strcpy(ppszLoopVars[iLoopVars - 1], loop_var);
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
			addIndent(&out_file, &iIndentIndex);
			fprintf(out_file, "%s;\n", loop_var);

			addIndent(&out_file, &iIndentIndex);
			fprintf(out_file, "while (%s)\n", condition);

			// Read the input file until the end of the for loop body
			while (fgets(line, sizeof(line), in_file) && strstr(line, "}") == NULL)
			{
				// Replace tabs with 3 spaces and write the line to the output file
				for (int i = 0; i < strlen(line); i++)
				{
					if (line[i] == '\t')
					{
						fputc(' ', out_file);
						fputc(' ', out_file);
						fputc(' ', out_file);
					}
					else
					{
						fputc(line[i], out_file);
					}
				}
			}
			// Write the increment to the output file
			addIndent(&out_file, &iIndentIndex);
			fprintf(out_file, "%s;\n", increment);

			addIndent(&out_file, &iIndentIndex);
			fprintf(out_file, "}\n");

			iIndentIndex--;
		}
		else
		{
			// Replace tabs with 3 spaces and write the line to the output file
			for (int i = 0; i < strlen(line); i++)
			{
				if (line[i] == '\t')
				{
					fputc(' ', out_file);
					fputc(' ', out_file);
					fputc(' ', out_file);
				}
				else
				{
					fputc(line[i], out_file);
				}
			}
		}
	}

	// Close the input and output files
	fclose(in_file);
	fclose(out_file);

	return 0;
}
