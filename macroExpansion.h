#ifndef MACRO_EXPANSION_H
#define MACRO_EXPANSION_H

#include <stdio.h>

#include "globals.h"

void expandMacros(char fileName[]);
void expandFileMacros(FILE *inputFile, FILE *outputFile);
boolean isEndOfMacro(char line[]);

#endif
