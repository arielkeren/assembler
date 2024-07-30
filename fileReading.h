/*
 * fileReading.h
 *
 * Contains the function prototypes for the functions in fileReading.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h> /* FILE. */

#include "globals.h" /* Typedefs. */

Boolean readFile(char fileName[], Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount);
Boolean readLines(char fileName[], FILE *file, Macro *macros, Word *code, Word *data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount);
Boolean handleLine(char fileName[], char line[], LineNumber lineNumber, Macro *macros, Word **code, Word **data, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels, WordCount *instructionCount, WordCount *dataCount);
Boolean handleLabel(char fileName[], char line[], LineNumber lineNumber, Macro *macros, FoundLabel **foundLabels, WordCount instructionCount, WordCount dataCount);
void handleOperation(char line[], LineNumber lineNumber, Word **code, UsedLabel **usedLabels, WordCount *instructionCount);

#endif
