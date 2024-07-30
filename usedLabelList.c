/*
 * usedLabelList.c
 *
 * Contains functions to interface with a used label list and to free it.
 * The used label list holds every label that is part of an instruction in the .am file.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "usedLabelList.h"

#include <stdlib.h> /* free. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

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
