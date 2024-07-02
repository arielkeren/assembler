#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>

#include "globals.h"

void readFile(char fileName[], macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void readLines(FILE *file, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleLine(char line[], unsigned lineNumber, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleOperation(char line[], word **code, usedLabel **usedLabels, unsigned *instructionCount);

#endif
