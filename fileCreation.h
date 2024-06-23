#ifndef FILE_CREATION_H
#define FILE_CREATION_H

#include <stdio.h>

#include "globals.h"

void makeObjectFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount);
void makeLabelFile(char fileName[], char extension[], label *labelList);
void insertWordList(FILE *file, word *wordList, unsigned startingIndex);
void insertLine(FILE *file, label *labelToInsert, size_t longest);
size_t getLongestLabel(label *labelList);

#endif
