#include "labelList.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addLabel(label **labelList, char labelName[], unsigned lineNumber) {
    label *newLabel;

    newLabel = allocate(sizeof(label));
    newLabel->name = labelName;
    newLabel->lineNumber = lineNumber;
    newLabel->next = *labelList;
    *labelList = newLabel;
}

boolean containsLabel(label *labelList, char labelName[]) {
    while (labelList != NULL) {
        if (strcmp(labelList->name, labelName) == EQUAL_STRINGS) {
            return TRUE;
        }

        labelList = labelList->next;
    }

    return FALSE;
}

unsigned char getLongestLabel(label *labelList) {
    unsigned char longest;
    unsigned char currentLength;

    longest = INITIAL_VALUE;

    while (labelList != NULL) {
        currentLength = (unsigned char)strlen(labelList->name);
        if (currentLength > longest) {
            longest = currentLength;
        }

        labelList = labelList->next;
    }

    return longest;
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
