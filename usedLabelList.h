#ifndef USED_LABEL_LIST_H
#define USED_LABEL_LIST_H

#include "globals.h"

void addUsedLabel(usedLabel **usedLabels, char labelName[], unsigned address, word *wordPointer);
void freeUsedLabelList(usedLabel *usedLabelList);
void freeUsedLabel(usedLabel *usedLabelToFree);

#endif
