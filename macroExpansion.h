#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h>

void expandMacros(char fileName[]);
void expandFileMacros(FILE *inputFile, FILE *outputFile);
int isEndOfMacro(char line[]);
char *getMacroName(char line[]);
int isValidMacroName(char line[]);
void removeEndingWhitespace(char line[]);
int isSingleWord(char line[]);

#endif
