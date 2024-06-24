#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "globals.h"

void secondPass(char fileName[], word *code, label *entryLabels, label *externLabels, usedLabel *usedLabels, unsigned instructionCount);
void secondPassFile(FILE *file, word *code, label *entryLabels, label *externLabels, usedLabel *usedLabels, unsigned instructionCount);
void findLabels(char line[], word *code, foundLabel **foundLabels, label *entryLabels, label *externLabels, usedLabel *usedLabels, unsigned instructionCount, unsigned *currentInstruction, unsigned *currentData);

#endif
