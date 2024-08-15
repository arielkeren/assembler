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
#include <string.h> /* strcmp. */

#include "globals.h" /* Constants and typedefs. */
#include "utils.h"   /* Allocating memory. */

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

void freeLabel(Label *label) {
    /* Free the name string. */
    free(label->name);
    /* Free the label. */
    free(label);
}
