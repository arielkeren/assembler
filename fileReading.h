#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>

#include "globals.h"

boolean readFile(char fileName[], macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
boolean readLines(FILE *file, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
boolean handleLine(char line[], unsigned lineNumber, macro *macros, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleOperation(char line[], word **code, usedLabel **usedLabels, unsigned *instructionCount);

#endif
