#include "labelList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addLabel(label **labelList, char labelName[]) {
    label *newLabel;

    newLabel = allocate(sizeof(label));
    newLabel->name = labelName;
    newLabel->address = 0;
    newLabel->next = *labelList;
    *labelList = newLabel;
}

void putAddress(label *labelList, unsigned address) {
    labelList->address = address;
}

void freeLabelList(label *labelList) {
    label *next;

    while (labelList != NULL) {
        next = labelList->next;
        freeLabel(labelList);
        labelList = next;
    }
}

void freeLabel(label *labelToFree) {
    free(labelToFree->name);
    free(labelToFree);
}
