/*
 * labelList.h
 *
 * Contains functions to interface with a label list, to search through it and finally to free it.
 * The 2 label lists used in the program contain all the labels marked entry and extern in the .am file.
 * They will eventually be used to generate the .ent and .ext files.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "labelList.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"

void addLabel(Label **labels, char labelName[], LineNumber lineNumber) {
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

Length getLongestLabel(Label *labels) {
    Length longest;
    Length currentLength;

    longest = INITIAL_VALUE;

    while (labels != NULL) {
        currentLength = (Length)strlen(labels->name);

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
