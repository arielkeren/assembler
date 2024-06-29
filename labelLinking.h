#ifndef LABEL_LINKING_H
#define LABEL_LINKING_H

#include <stdio.h>

#include "globals.h"

void linkLabels(label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount);
void generateEntFile(char fileName[], label *entryLabels, foundLabel *foundLabels, unsigned instructionCount);
void generateExtFile(char fileName[], label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels);
void insertEntryLabels(FILE *file, label *entryLabels, foundLabel *foundLabels, unsigned instructionCount, unsigned char longest);
void insertExternLabels(FILE *file, label *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned char longest);
void insertLabel(FILE *file, char labelName[], unsigned address, unsigned char longest);

#endif
