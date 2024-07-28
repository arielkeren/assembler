#include "foundLabelList.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addFoundLabel(FoundLabel **labels, char labelName[]) {
    FoundLabel *newLabel;

    newLabel = allocate(sizeof(FoundLabel));
    newLabel->name = labelName;
    newLabel->isData = FALSE;
    newLabel->next = *labels;
    *labels = newLabel;
}

void markAsData(FoundLabel *label) {
    label->isData = TRUE;
}

void setAddress(FoundLabel *label, Address newAddress) {
    label->address = newAddress;
}

FoundLabel *getFoundLabel(FoundLabel *labels, char labelName[]) {
    while (labels != NULL) {
        if (strcmp(labels->name, labelName) == EQUAL_STRINGS) {
            return labels;
        }

        labels = labels->next;
    }

    return NULL;
}

void freeFoundLabelList(FoundLabel *labels) {
    FoundLabel *next;

    while (labels != NULL) {
        next = labels->next;
        freeFoundLabel(labels);
        labels = next;
    }
}

void freeFoundLabel(FoundLabel *label) {
    free(label->name);
    free(label);
}
