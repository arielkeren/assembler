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

/**
 * Adds a new used label to the given list of used labels with the given name, address, line number and word pointer.
 * IMPORTANT: The caller must free the new used label.
 *
 * @param labels The list of used labels.
 * @param labelName The new used label's name.
 * @param address The new used label's word address.
 * @param lineNumber The new used label's line number.
 * @param wordPointer The new used label's word pointer.
 */
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

/**
 * Frees a used label list.
 *
 * @param labels The used label list to free.
 */
void freeUsedLabelList(UsedLabel *labels) {
    UsedLabel *next;

    while (labels != NULL) {
        next = labels->next;
        freeUsedLabel(labels);
        labels = next;
    }
}

/**
 * Frees a single used label.
 *
 * @param label The used label to free.
 */
void freeUsedLabel(UsedLabel *label) {
    free(label->name);
    free(label);
}
