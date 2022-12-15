#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char pszInputFile[] = "input.txt";
    char pszOutputFile[] = "output.txt";

    // Open the input file in read-only mode
    FILE *pfInput = fopen(pszInputFile, "r");
    if (pfInput == NULL)
    {
        printf("Error opening input file: %s\n", pszInputFile);
        return 1;
    }

    // Open the output file in write-only mode
    FILE *pfOutput = fopen(pszOutputFile, "w");
    if (pfOutput == NULL)
    {
        printf("Error opening output file: %s\n", pszOutputFile);
        return 1;
    }

    // Read one character at a time from the input file
    char ch;
    while ((ch = fgetc(pfInput)) != EOF)
    {
        // Convert the character to its ASCII representation in hexadecimal
        char pszHex[3];
        snprintf(pszHex, 3, "%02X", ch);
        // Write the hexadecimal representation to the output file
        fputs(pszHex, pfOutput);
    }

    // Close the input and output files
    fclose(pfInput);
    fclose(pfOutput);

    return 0;
}