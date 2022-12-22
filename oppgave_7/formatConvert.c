#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "include/formatConvert.h"

void addIndent(FILE **_ppfOutputCode, int _iIndentIndex, int _iIndentSize)
{
    for (size_t i = 0; i < (_iIndentIndex * _iIndentSize); i++)
    {
        fputc(' ', *_ppfOutputCode);
    }
}

int formatCode(FILE **_ppfInputCode, FILE **_ppfOutputCode, int *_piIndentIndex, int _iStartIndentIndex, int *_piNumLoopVars, char ***_pppszLoopVars, char (*pszCodeLine)[])
{
    char szFullCodeLine[CODE_LINE_SIZE];
    strcpy(szFullCodeLine, *pszCodeLine);

    // Add or remove indent index based on '{}'.
    if (strstr(szFullCodeLine, "{") != NULL && strstr(szFullCodeLine, "for (") == NULL)
    {
        (*_piIndentIndex)++;
    }
    if (strstr(szFullCodeLine, "}") != NULL)
    {
        (*_piIndentIndex)--;
    }

    // Check if the line is a for loop.
    if (strstr(szFullCodeLine, "for (") != NULL)
    {
        // Extract the loop variable, condition, and increment from the for loop
        char *start = strchr(*pszCodeLine, '(');
        char *end = strchr(*pszCodeLine, ')');
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
        for (size_t i = 0; i < *_piNumLoopVars; i++)
        {
            if (strcmp((*_pppszLoopVars)[i], loop_var) == 0)
            {
                bLoopVarExists = true;
            }
        }

        // Checking if the variable has been declared. If so, do not redeclare it.
        if (!bLoopVarExists)
        {
            // Save loop var in list.
            (*_piNumLoopVars)++;
            (*_pppszLoopVars) = (char **)realloc((*_pppszLoopVars), (*_piNumLoopVars) * sizeof(char **));
            (*_pppszLoopVars)[(*_piNumLoopVars) - 1] = (char *)malloc(100 * sizeof(char));
            strcpy((*_pppszLoopVars)[(*_piNumLoopVars) - 1], loop_var);
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
        addIndent(_ppfOutputCode, (*_piIndentIndex), INDENT_SIZE);
        fprintf(*_ppfOutputCode, "%s;\n", loop_var);

        // Print while code line based on if the '{' is on the same line.
        if(strstr(szFullCodeLine, "{") != NULL) {
            addIndent(_ppfOutputCode, (*_piIndentIndex), INDENT_SIZE);
            fprintf(*_ppfOutputCode, "while (%s) {\n", condition);
            
            // Add indent when entered the loop.
            (*_piIndentIndex)++;
        } else {
            addIndent(_ppfOutputCode, (*_piIndentIndex), INDENT_SIZE);
            fprintf(*_ppfOutputCode, "while (%s)\n", condition);
        }

        // Read the input file until the end of the for loop body
        while (fgets(*pszCodeLine, CODE_LINE_SIZE, *_ppfInputCode) && strstr(*pszCodeLine, "}") == NULL)
        {
            // Check recursivly for multiple for loops.
            formatCode(_ppfInputCode, _ppfOutputCode, _piIndentIndex, *_piIndentIndex, _piNumLoopVars, _pppszLoopVars, pszCodeLine);
        }

        // Write the increment to the output file
        addIndent(_ppfOutputCode, *_piIndentIndex, INDENT_SIZE);
        fprintf(*_ppfOutputCode, "%s;\n", increment);

        (*_piIndentIndex)--;

        addIndent(_ppfOutputCode, *_piIndentIndex, INDENT_SIZE);
        fprintf(*_ppfOutputCode, "}\n");
    }
    else
    {
        // Replace tabs with 3 spaces and write the line to the output file
        for (int i = 0; i < strlen(*pszCodeLine); i++)
        {
            if ((*pszCodeLine)[i] == '\t')
            {
                fputc(' ', *_ppfOutputCode);
                fputc(' ', *_ppfOutputCode);
                fputc(' ', *_ppfOutputCode);
            }
            else
            {
                fputc((*pszCodeLine)[i], *_ppfOutputCode);
            }
        }
    }

    if (strstr(*pszCodeLine, "}") != NULL && (*_piIndentIndex) == _iStartIndentIndex)
    {
        return 0;
    }

    return 0;
}
