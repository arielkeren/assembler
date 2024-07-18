#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>

#include "globals.h"

boolean readFile(char fileName[], macro *macros, word *code, word *data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
boolean readLines(char fileName[], FILE *file, macro *macros, word *code, word *data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
boolean handleLine(char fileName[], char line[], unsigned lineNumber, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleOperation(char line[], unsigned lineNumber, word **code, usedLabel **usedLabels, unsigned *instructionCount);

#endif
