#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addLabel(label **labels, char labelName[]) {
    label *newLabel;

    newLabel = allocate(sizeof(label));
    newLabel->name = labelName;
    newLabel->next = *labels;
    *labels = newLabel;
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
