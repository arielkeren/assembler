#include "foundLabelList.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addFoundLabel(foundLabel **foundLabelList, char labelName[]) {
    foundLabel *newFoundLabel;

    newFoundLabel = allocate(sizeof(foundLabel));
    newFoundLabel->name = labelName;
    newFoundLabel->isData = FALSE;
    newFoundLabel->next = *foundLabelList;
    *foundLabelList = newFoundLabel;
}

void markAsData(foundLabel *foundLabelToModify) {
    foundLabelToModify->isData = TRUE;
}

void setAddress(foundLabel *foundLabelToModify, unsigned newAddress) {
    foundLabelToModify->address = newAddress;
}

foundLabel *getFoundLabel(foundLabel *foundLabelList, char labelName[]) {
    while (foundLabelList != NULL) {
        if (strcmp(foundLabelList->name, labelName) == 0) {
            return foundLabelList;
        }

        foundLabelList = foundLabelList->next;
    }

    return NULL;
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
