#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h>

#include "globals.h"

void expandMacros(char fileName[], macro **macros);
void expandFileMacros(FILE *inputFile, FILE *outputFile, macro **macros);
void expandLineMacros(FILE *inputFile, FILE *outputFile, macro **macros, char line[], unsigned lineNumber, boolean *isInsideMacro);
boolean isEndOfMacro(char line[]);

#endif
