/*
 * foundLabelList.h
 *
 * Contains the function prototypes for the functions in foundLabelList.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FOUND_LABEL_LIST_H
#define FOUND_LABEL_LIST_H

#include "globals.h" /* Typedefs. */

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
void addFoundLabel(FoundLabel **labels, char labelName[]);

/**
 * Marks a found label as data.
 *
 * Assumes that the given label pointer is not NULL.
 *
 * @param label The found label to mark as data.
 */
void markAsData(FoundLabel *label);

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
void setAddress(FoundLabel *label, Address newAddress);

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
FoundLabel *getFoundLabel(FoundLabel *labels, char labelName[]);

/**
 * Frees a list of found labels.
 *
 * Assumes that the given labels have not been freed yet.
 *
 * @param labels The list of found labels to free.
 */
void freeFoundLabelList(FoundLabel *labels);

/**
 * Frees a single found label.
 *
 * Assumes that the given label is not NULL and has not been freed yet.
 *
 * @param label The found label to free.
 */
void freeFoundLabel(FoundLabel *label);

#endif
