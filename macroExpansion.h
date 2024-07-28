#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h>

#include "globals.h"

Boolean expandMacros(char fileName[], Macro **macros);
Boolean expandFileMacros(FILE *inputFile, FILE *outputFile, Macro **macros, char fileName[]);
Boolean expandLineMacros(FILE *inputFile, FILE *outputFile, Macro **macros, char fileName[], char line[], LineNumber lineNumber, Boolean *isInsideMacro);
Boolean isEndOfMacro(char line[]);

#endif
