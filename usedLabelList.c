#include "usedLabelList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addUsedLabel(usedLabel **usedLabels, char labelName[], word *wordPointer) {
    usedLabel *newUsedLabel;

    newUsedLabel = allocate(sizeof(usedLabel));
    newUsedLabel->name = labelName;
    newUsedLabel->wordPointer = wordPointer;
    newUsedLabel->next = *usedLabels;
    *usedLabels = newUsedLabel;
}

void freeUsedLabelList(usedLabel *usedLabelList) {
    label *next;

    while (usedLabelList != NULL) {
        next = usedLabelList->next;
        freeLabel(usedLabelList);
        usedLabelList = next;
    }
}

void freeUsedLabel(usedLabel *usedLabelToFree) {
    free(usedLabelToFree->name);
    free(usedLabelToFree);
}
