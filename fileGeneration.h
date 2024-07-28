#ifndef FILE_GENERATION_H
#define FILE_GENERATION_H

#include <stdio.h>

#include "globals.h"

void generateObFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount);
boolean generateEntFile(char fileName[], label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, boolean shouldGenerate);
boolean generateExtFile(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, boolean shouldGenerate);
void insertWordList(FILE *file, word *wordList, address startingAddress);
void insertLabel(FILE *file, char labelName[], address labelAddress, unsigned char longest);

#endif
