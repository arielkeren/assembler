#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"

void secondPass(label *entryLabels, externLabel *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount);
void insertLabelAddresses(usedLabel *usedLabels, externLabel *externLabels, foundLabel *foundLabels, unsigned instructionCount);
void fillEntryLabels(label *entryLabels, foundLabel *foundLabels, unsigned instructionCount);
void fillExternLabels(externLabel *externLabels, usedLabel *usedLabels);

#endif
