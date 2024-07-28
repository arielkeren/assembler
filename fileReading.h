#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>

#include "globals.h"

Boolean readFile(char fileName[], Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount);
Boolean readLines(char fileName[], FILE *file, Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount);
Boolean handleLine(char fileName[], char line[], LineNumber lineNumber, Macro *macros, Word **code, Word **data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount);
Boolean handleLabel(char fileName[], char line[], LineNumber lineNumber, Macro *macros, FoundLabel **foundLabels, WordCount instructionCount, WordCount dataCount);
void handleOperation(char line[], LineNumber lineNumber, Word **code, UsedLabel **usedLabels, WordCount *instructionCount);

#endif
