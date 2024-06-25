#include "usedLabelList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addUsedLabel(usedLabel **usedLabels, char labelName[], unsigned address, word *wordPointer) {
    usedLabel *newUsedLabel;

    newUsedLabel = allocate(sizeof(usedLabel));
    newUsedLabel->name = labelName;
    newUsedLabel->address = address;
    newUsedLabel->wordPointer = wordPointer;
    newUsedLabel->next = *usedLabels;
    *usedLabels = newUsedLabel;
}

void freeUsedLabelList(usedLabel *usedLabelList) {
    usedLabel *next;

    while (usedLabelList != NULL) {
        next = usedLabelList->next;
        freeUsedLabel(usedLabelList);
        usedLabelList = next;
    }
}

void freeUsedLabel(usedLabel *usedLabelToFree) {
    free(usedLabelToFree->name);
    free(usedLabelToFree);
}
