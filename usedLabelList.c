#include "usedLabelList.h"

#include <stdlib.h>

#include "globals.h"
#include "utils.h"

void addUsedLabel(UsedLabel **labels, char labelName[], Address address, LineNumber lineNumber, Word *wordPointer) {
    UsedLabel *newLabel;

    newLabel = allocate(sizeof(UsedLabel));
    newLabel->name = labelName;
    newLabel->address = address;
    newLabel->lineNumber = lineNumber;
    newLabel->wordPointer = wordPointer;

    newLabel->next = *labels;
    *labels = newLabel;
}

void freeUsedLabelList(UsedLabel *labels) {
    UsedLabel *next;

    while (labels != NULL) {
        next = labels->next;
        freeUsedLabel(labels);
        labels = next;
    }
}

void freeUsedLabel(UsedLabel *label) {
    free(label->name);
    free(label);
}
