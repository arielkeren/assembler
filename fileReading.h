#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>

#include "globals.h"

Boolean readFile(char fileName[], Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
Boolean readLines(char fileName[], FILE *file, Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
Boolean handleLine(char fileName[], char line[], unsigned lineNumber, Macro *macros, Word **code, Word **data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, unsigned *instructionCount, unsigned *dataCount);
Boolean handleLabel(char fileName[], char line[], unsigned lineNumber, Macro *macros, FoundLabel **foundLabels, unsigned instructionCount, unsigned dataCount);
void handleOperation(char line[], unsigned lineNumber, Word **code, UsedLabel **usedLabels, unsigned *instructionCount);

#endif
