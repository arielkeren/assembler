#ifndef FILE_GENERATION_H
#define FILE_GENERATION_H

#include <stdio.h>

#include "globals.h"

void generateObFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount);
boolean generateEntFile(char fileName[], label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, boolean shouldGenerate);
boolean generateExtFile(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, boolean shouldGenerate);
void insertWordList(FILE *file, word *wordList, unsigned startingAddress);
boolean insertEntryLabels(FILE *file, label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, unsigned char longest);
boolean insertExternLabels(FILE *file, label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned char longest);
void insertLabel(FILE *file, char labelName[], unsigned address, unsigned char longest);

#endif
