#include "labelList.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addLabel(Label **labels, char labelName[], unsigned lineNumber) {
    Label *newLabel;

    newLabel = allocate(sizeof(Label));
    newLabel->name = labelName;
    newLabel->lineNumber = lineNumber;
    newLabel->next = *labels;
    *labels = newLabel;
}

Boolean containsLabel(Label *labels, char labelName[]) {
    while (labels != NULL) {
        if (strcmp(labels->name, labelName) == EQUAL_STRINGS) {
            return TRUE;
        }

        labels = labels->next;
    }

    return FALSE;
}

unsigned char getLongestLabel(Label *labels) {
    unsigned char longest;
    unsigned char currentLength;

    longest = INITIAL_VALUE;

    while (labels != NULL) {
        currentLength = (unsigned char)strlen(labels->name);
        if (currentLength > longest) {
            longest = currentLength;
        }

        labels = labels->next;
    }

    return longest;
}

void freeLabelList(Label *labels) {
    Label *next;

    while (labels != NULL) {
        next = labels->next;
        freeLabel(labels);
        labels = next;
    }
}

void freeLabel(Label *label) {
    free(label->name);
    free(label);
}
