/*
 * labelList.h
 *
 * Contains functions to interface with a label list, to search through it and
 * finally to free it.
 * The 2 label lists used in the program contain all the labels marked entry and
 * extern in the .am file.
 * They will eventually be used to generate the .ent and .ext files.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#include "labelList.h"

#include <stdlib.h> /* free. */
#include <string.h> /* strcmp, strlen. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

/**
 * Adds a new label to the label list.
 * IMPORTANT: The caller must free the new label.
 *
 * Assumes that the pointer to the labels is not NULL.
 * Assumes that the label name is not NULL and is null-terminated.
 *
 * @param labels The label list.
 * @param labelName The new label's name.
 * @param lineNumber The new label's line number.
 */
void addLabel(Label **labels, char labelName[], LineNumber lineNumber) {
    Label *newLabel;

    newLabel = allocate(sizeof(Label));
    newLabel->name = labelName;
    newLabel->lineNumber = lineNumber;
    newLabel->next = *labels;
    *labels = newLabel;
}

/**
 * Searches for the given label name in the given label list.
 * Returns whether or not it has been found.
 *
 * Assumes that the given label name is not NULL and is null-terminated.
 *
 * @param labels The label list.
 * @param labelName The name of the label to search for.
 * @return TRUE if the label exists, FALSE otherwise.
 */
Boolean containsLabel(Label *labels, char labelName[]) {
    while (labels != NULL) {
        if (strcmp(labels->name, labelName) == EQUAL_STRINGS) {
            return TRUE;
        }

        labels = labels->next;
    }

    return FALSE;
}

/**
 * Computes and returns the length of the longest label in the given label list.
 *
 * @param labels The label list to compute the length of the longest label in.
 * @return The length of the longest label in the given label list.
 */
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

/**
 * Frees a list of labels.
 *
 * Assumes that the given labels have not been freed yet.
 *
 * @param labels The label list to free.
 */
void freeLabelList(Label *labels) {
    Label *next;

    while (labels != NULL) {
        next = labels->next;
        freeLabel(labels);
        labels = next;
    }
}

/**
 * Frees a single label.
 *
 * Assumes that the given label is not NULL and has not been freed yet.
 *
 * @param label The label to free.
 */
void freeLabel(Label *label) {
    free(label->name);
    free(label);
}
