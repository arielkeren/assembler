/*
 * foundLabelList.c
 *
 * Contains functions to interface with a found label list, to search through it and finally to free it.
 * The found label list contains all the label definitions in the .am file.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "foundLabelList.h"

#include <stdlib.h> /* free. */
#include <string.h> /* strcmp. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

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
