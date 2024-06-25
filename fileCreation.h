#ifndef FILE_CREATION_H
#define FILE_CREATION_H

#include <stdio.h>

#include "globals.h"

void generateObFile(char fileName[], word *code, word *data, unsigned instructionCount, unsigned dataCount);
void generateEntFile(char fileName[], label *entryLabels);
void generateExtFile(char fileName[], externLabel *externLabels);
void insertWordList(FILE *file, word *wordList, unsigned startingIndex);
void insertEntryLabels(FILE *file, label *entryLabels);
void insertExternLabels(FILE *file, externLabel *externLabels);
void insertUses(FILE *file, externLabel *labelToInsert, size_t longest);
void insertLabel(FILE *file, char labelName[], unsigned address, size_t longest);
size_t getLongestEntryLabel(label *entryLabels);
size_t getLongestExternLabel(externLabel *externLabels);

#endif
