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

/**
 * Adds a new found label to a list of found labels.
 * IMPORTANT: The caller must free the new found label.
 *
 * Assumes that the pointer to the labels is not NULL.
 * Assumes that the label name is not NULL and is null-terminated.
 *
 * @param labels The list of found labels.
 * @param labelName The new found label's name.
 */
void addFoundLabel(FoundLabel **labels, char labelName[]) {
    FoundLabel *newLabel;

    newLabel = allocate(sizeof(FoundLabel));
    newLabel->name = labelName;
    newLabel->isData = FALSE;
    newLabel->next = *labels;
    *labels = newLabel;
}

/**
 * Marks a found label as data.
 *
 * Assumes that the given label pointer is not NULL.
 *
 * @param label The found label to mark as data.
 */
void markAsData(FoundLabel *label) { label->isData = TRUE; }

/**
 * Sets the address of a found label.
 *
 * Assumes that the given label pointer is not NULL.
 * Assumes that the new address is valid, so that the final address will fit in
 * the made-up memory.
 *
 * @param label The found label to set the address of.
 * @param newAddress The new address.
 */
void setAddress(FoundLabel *label, Address newAddress) {
    label->address = newAddress;
}

/**
 * Searches and returns a found label from a list of found labels with its name.
 * Returns NULL if no found label with the given name exists.
 *
 * Assumes that the given label name is not NULL and is null-terminated.
 *
 * @param labels The list of found labels.
 * @param labelName The name of the label to search for.
 * @return The found label with the given name or NULL if not found.
 */
FoundLabel *getFoundLabel(FoundLabel *labels, char labelName[]) {
    while (labels != NULL) {
        if (strcmp(labels->name, labelName) == EQUAL_STRINGS) {
            return labels;
        }

        labels = labels->next;
    }

    return NULL;
}

/**
 * Frees a list of found labels.
 *
 * Assumes that the given labels have not been freed yet.
 *
 * @param labels The list of found labels to free.
 */
void freeFoundLabelList(FoundLabel *labels) {
    FoundLabel *next;

    while (labels != NULL) {
        next = labels->next;
        freeFoundLabel(labels);
        labels = next;
    }
}

/**
 * Frees a single found label.
 *
 * Assumes that the given label is not NULL and has not been freed yet.
 *
 * @param label The found label to free.
 */
void freeFoundLabel(FoundLabel *label) {
    free(label->name);
    free(label);
}
