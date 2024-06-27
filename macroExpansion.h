#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h>

#include "globals.h"

void expandMacros(char fileName[]);
void expandFileMacros(FILE *inputFile, FILE *outputFile);
boolean isEndOfMacro(char line[]);
char *getMacroName(char line[]);
boolean isValidMacroName(char line[]);
void removeEndingWhitespace(char line[]);
boolean isSingleWord(char line[]);

#endif
