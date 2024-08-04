/*
 * freeingLogic.h
 *
 * Contains the function prototypes for the functions in freeingLogic.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef FREEING_LOGIC_H
#define FREEING_LOGIC_H

#include "globals.h" /* Typedefs. */

/**
 * Frees all the memory used by all the linked lists in the program.
 *
 * Assumes that this function is called only in case of an allocation failure.
 * Assumes that the pointers to all the linked lists in the program have been
 * set.
 */
void freeAll();

/**
 * Sets the pointers to each linked list in memory to free it, in case of an
 * allocation failure.
 *
 * Assumes that all the arguments point to the linked lists that need to be
 * freed.
 *
 * @param code The code word list.
 * @param data The data word list.
 * @param macros The macro table.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 */
void setToFree(Word **code, Word **data, Macro **macros, Label **entryLabels,
               Label **externLabels, UsedLabel **usedLabels,
               FoundLabel **foundLabels);

/**
 * Manages the logic for keeping pointers to the linked lists in the program.
 * Can free them all in case of an allocation failure.
 *
 * Assumes that all the pointer arguments point to the linked lists that need to
 * be freed.
 * Assumes that this function is called with shouldFree = TRUE only in case of
 * an allocation failure.
 * Assumes that the pointers to all the linked lists in the program have been
 * set, if this function is called with shouldFree = TRUE.
 *
 * @param shouldFree Frees the lists if TRUE, otherwise sets them to the given
 * pointers.
 * @param code The code word list.
 * @param data The data word list.
 * @param macros The macro table.
 * @param entryLabels The list of entry labels.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 */
void freeingLogic(Boolean shouldFree, Word **code, Word **data, Macro **macros,
                  Label **entryLabels, Label **externLabels,
                  UsedLabel **usedLabels, FoundLabel **foundLabels);

/**
 * Frees all the linked lists in the program.
 *
 * Assumes that the pointer received is not NULL.
 *
 * @param toFree The pointers to the linked lists to free.
 */
void freeLists(ToFree *toFree);

#endif
