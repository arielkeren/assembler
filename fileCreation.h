#ifndef FILE_CREATION_H
#define FILE_CREATION_H

#include <stdio.h>

#include "globals.h"

void generateObFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount);
void insertWordList(FILE *file, word *wordList, unsigned startingAddress);

#endif
