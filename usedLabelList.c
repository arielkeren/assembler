/*
 * usedLabelList.c
 *
 * Contains functions to interface with a used label list and to free it.
 * The used label list holds every label that is part of an instruction in the
 * .am file.
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

void addUsedLabel(UsedLabel **labels, char labelName[], Address address,
                  LineNumber lineNumber, Word *wordPointer) {
    UsedLabel *newLabel; /* The new used label. */

    /* Allocate enough memory for the new used label. */
    newLabel = allocate(sizeof(UsedLabel));

    /* Set the new used label's properties. */
    newLabel->name = labelName;
    newLabel->address = address;
    newLabel->lineNumber = lineNumber;
    newLabel->wordPointer = wordPointer;

    /* Add the new used label to the head of the list. */
    newLabel->next = *labels;
    *labels = newLabel;
}

void freeUsedLabelList(UsedLabel *labels) {
    UsedLabel *next; /* The next used label in the list. */

    /* Loop over each label in the list and free it. */
    while (labels != NULL) {
        /* Store the next label. */
        next = labels->next;
        /* Free the current label. */
        freeUsedLabel(labels);
        /* Move on to the next label. */
        labels = next;
    }
}

void freeUsedLabel(UsedLabel *label) {
    /* Free the name string. */
    free(label->name);
    /* Free the label. */
    free(label);
}
