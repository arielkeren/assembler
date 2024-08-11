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

/**
 * Marks a found label as data.
 *
 * Assumes that the given label pointer is not NULL.
 *
 * @param label The found label to mark as data.
 */
void markAsData(FoundLabel *label) {
    /* Set the isData property. */
    label->isData = TRUE;
}

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
    /* Set the address property. */
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

/**
 * Frees a list of found labels.
 *
 * Assumes that the given labels have not been freed yet.
 *
 * @param labels The list of found labels to free.
 */
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

/**
 * Frees a single found label.
 *
 * Assumes that the given label is not NULL and has not been freed yet.
 *
 * @param label The found label to free.
 */
void freeFoundLabel(FoundLabel *label) {
    /* Free the name string. */
    free(label->name);
    /* Free the label. */
    free(label);
}
