#ifndef __FORMAT_CONVERT_H__
#define __FORMAT_CONVERT_H__

#include <stdio.h>

#define INDENT_SIZE 3
#define CODE_LINE_SIZE 1024

int formatCode(FILE **_ppfInputCode, FILE **_ppfOutputCode, int *_piIndentIndex, int _iStartIndentIndex, int *_piNumLoopVars, char ***_pppszLoopVars, char (*pszCodeLine)[]);
void addIndent(FILE **_ppfOutFile, int _piIndentIndex, int _iIndentSize);

#endif