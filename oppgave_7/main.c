#include <stdio.h>
#include <string.h>

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

    // Read the input file line by line and convert for loops to while loops
    char line[1000];
    while (fgets(line, sizeof(line), in_file))
    {
        // Check if the line is a for loop
        if (strstr(line, "for (") != NULL)
        {
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

            // Write the loop variable and condition to the output file
            fprintf(out_file, "%s;\n", loop_var);
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
            fprintf(out_file, "%s;\n", increment);
            fprintf(out_file, "}\n");
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
