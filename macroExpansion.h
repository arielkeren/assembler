#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h>

#include "globals.h"

boolean expandMacros(char fileName[], macro **macros);
boolean expandFileMacros(FILE *inputFile, FILE *outputFile, macro **macros, char fileName[]);
boolean expandLineMacros(FILE *inputFile, FILE *outputFile, macro **macros, char fileName[], char line[], unsigned lineNumber, boolean *isInsideMacro);
boolean isEndOfMacro(char line[]);

#endif
