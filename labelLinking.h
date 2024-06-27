#ifndef LABEL_LINKING_H
#define LABEL_LINKING_H

#include "globals.h"

void linkLabels(label *entryLabels, externLabel *externLabels, usedLabel *usedLabels, foundLabel *foundLabels, unsigned instructionCount);
void insertLabelAddresses(usedLabel *usedLabels, externLabel *externLabels, foundLabel *foundLabels, unsigned instructionCount);
void fillEntryLabels(label *entryLabels, foundLabel *foundLabels, unsigned instructionCount);
void fillExternLabels(externLabel *externLabels, usedLabel *usedLabels);

#endif
