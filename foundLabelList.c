#include "foundLabelList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addFoundLabel(foundLabel **foundLabelList, char labelName[]) {
    foundLabel *newFoundLabel;

    newFoundLabel = allocate(sizeof(foundLabel));
    newFoundLabel->name = labelName;
    newFoundLabel->next = *foundLabelList;
    *foundLabelList = newFoundLabel;
}

void freeFoundLabelList(foundLabel *foundLabelList) {
    foundLabel *next;

    while (foundLabelList != NULL) {
        next = foundLabelList->next;
        freeFoundLabel(foundLabelList);
        foundLabelList = next;
    }
}

void freeFoundLabel(foundLabel *foundLabelToFree) {
    free(foundLabelToFree->name);
    free(foundLabelToFree);
}
