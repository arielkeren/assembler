#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>

#include "globals.h"

void readFile(char fileName[], word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void readLines(FILE *file, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleLine(char line[], unsigned lineNumber, word **code, word **data, label **entryLabels, label **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleOperation(char line[], word **code, usedLabel **usedLabels, unsigned *instructionCount);

#endif
