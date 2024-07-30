/*
 * fileGeneration.h
 *
 * Contains the function prototypes for the functions in fileGeneration.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FILE_GENERATION_H
#define FILE_GENERATION_H

#include <stdio.h> /* FILE. */

#include "globals.h" /* Typedefs. */

void generateObFile(char fileName[], Word *code, Word *data, WordCount instructionCount, WordCount dataCount);
Boolean generateEntFile(char fileName[], Label *entryLabels, FoundLabel *foundLabels, WordCount instructionCount, Boolean shouldGenerate);
Boolean generateExtFile(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, Boolean shouldGenerate);
void insertWordList(FILE *file, Word *wordList, Address startingAddress);
void insertLabel(FILE *file, char labelName[], Address address, Length longest);

#endif
