#ifndef USED_LABEL_LIST_H
#define USED_LABEL_LIST_H

#include "globals.h"

void addUsedLabel(UsedLabel **labels, char labelName[], Address address, unsigned lineNumber, Word *wordPointer);
void freeUsedLabelList(UsedLabel *labels);
void freeUsedLabel(UsedLabel *label);

#endif
