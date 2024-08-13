/*
 * foundLabelList.c
 *
 * Contains functions to interface with a found label list, to search through it
 * and finally to free it.
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
    FoundLabel *newLabel; /* The new found label. */

    /* Allocate enough memory for the new found label. */
    newLabel = allocate(sizeof(FoundLabel));

    /* Set the new found label's properties. */
    newLabel->name = labelName;
    newLabel->isData = FALSE;

    /* Add the new found label to the head of the list. */
    newLabel->next = *labels;
    *labels = newLabel;
}

void markAsData(FoundLabel *label) {
    /* Set the isData property. */
    label->isData = TRUE;
}

void setAddress(FoundLabel *label, Address newAddress) {
    /* Set the address property. */
    label->address = newAddress;
}

FoundLabel *getFoundLabel(FoundLabel *labels, char labelName[]) {
    /* Loop over the labels to try to find the desired one. */
    while (labels != NULL) {
        /* Compare the names. */
        if (strcmp(labels->name, labelName) == EQUAL_STRINGS) {
            /* The label has been found. */
            return labels;
        }

        /* Try checking the next label. */
        labels = labels->next;
    }

    /* The label has not been found. */
    return NULL;
}

void freeFoundLabelList(FoundLabel *labels) {
    FoundLabel *next; /* The next label in the list. */

    /* Loop over the labels in the list and free each one. */
    while (labels != NULL) {
        /* Store the next label. */
        next = labels->next;
        /* Free the current label. */
        freeFoundLabel(labels);
        /* Move on to the next label. */
        labels = next;
    }
}

void freeFoundLabel(FoundLabel *label) {
    /* Free the name string. */
    free(label->name);
    /* Free the label. */
    free(label);
}
