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
    Label *newLabel; /* The new label. */

    /* Allocate enough memory for the new label. */
    newLabel = allocate(sizeof(Label));

    /* Set the new label's properties. */
    newLabel->name = labelName;
    newLabel->lineNumber = lineNumber;

    /* Add the new label to the head of the list. */
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
    /* Loop over the labels to try to find the desired one. */
    while (labels != NULL) {
        /* Compare the label names. */
        if (strcmp(labels->name, labelName) == EQUAL_STRINGS) {
            /* The label has been found. */
            return TRUE;
        }

        /* Try checking the next label. */
        labels = labels->next;
    }

    /* The label has not been found. */
    return FALSE;
}

/**
 * Computes and returns the length of the longest label in the given label list.
 *
 * @param labels The label list to compute the length of the longest label in.
 * @return The length of the longest label in the given label list.
 */
Length getLongestLabel(Label *labels) {
    Length longest;       /* The length of the longest label so far. */
    Length currentLength; /* The length of the current label. */

    /* Intialize the longest value to the shortest possible. */
    longest = INITIAL_VALUE;

    /* Loop over the labels and check each one's length. */
    while (labels != NULL) {
        /* Get the length of the current label. */
        currentLength = (Length)strlen(labels->name);

        /* Check if the current label is longer than the longest so far. */
        if (currentLength > longest) {
            /* Replace the longest. */
            longest = currentLength;
        }

        /* Try checking the next label. */
        labels = labels->next;
    }

    /* Return the length of the longest label. */
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
    Label *next; /* The next label. */

    /* Loop over the labels and free each one. */
    while (labels != NULL) {
        /* Store the next label. */
        next = labels->next;
        /* Free the current label. */
        freeLabel(labels);
        /* Move on to the next label. */
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
    /* Free the name string. */
    free(label->name);
    /* Free the label. */
    free(label);
}
