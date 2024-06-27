#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>

#include "globals.h"

void firstPass(char fileName[], word **code, word **data, label **entryLabels, externLabel **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void firstPassFile(FILE *file, word **code, word **data, label **entryLabels, externLabel **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleLine(char line[], word **code, word **data, label **entryLabels, externLabel **externLabels, usedLabel **usedLabels, foundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
void handleOperation(char line[], word **code, usedLabel **usedLabels, unsigned *instructionCount);

#endif
