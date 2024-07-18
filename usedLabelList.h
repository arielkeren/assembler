#ifndef USED_LABEL_LIST_H
#define USED_LABEL_LIST_H

#include "globals.h"

void addUsedLabel(usedLabel **usedLabels, char labelName[], unsigned address, unsigned lineNumber, word *wordPointer);
void freeUsedLabelList(usedLabel *usedLabelList);
void freeUsedLabel(usedLabel *usedLabelToFree);

#endif
