/*
 * labelList.h
 *
 * Contains the function prototypes for the functions in labelList.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef LABEL_LIST_H
#define LABEL_LIST_H

#include "globals.h" /* Typedefs. */

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
void addLabel(Label **labels, char labelName[], LineNumber lineNumber);

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
Boolean containsLabel(Label *labels, char labelName[]);

/**
 * Computes and returns the length of the longest label in the given label list.
 *
 * @param labels The label list to compute the length of the longest label in.
 * @return The length of the longest label in the given label list.
 */
Length getLongestLabel(Label *labels);

/**
 * Frees a list of labels.
 *
 * Assumes that the given labels have not been freed yet.
 *
 * @param labels The label list to free.
 */
void freeLabelList(Label *labels);

/**
 * Frees a single label.
 *
 * Assumes that the given label is not NULL and has not been freed yet.
 *
 * @param label The label to free.
 */
void freeLabel(Label *label);

#endif
