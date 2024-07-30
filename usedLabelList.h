/*
 * usedLabelList.h
 *
 * Contains the function prototypes for the functions in usedLabelList.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef USED_LABEL_LIST_H
#define USED_LABEL_LIST_H

#include "globals.h" /* Typedefs. */

void addUsedLabel(UsedLabel **labels, char labelName[], Address address, LineNumber lineNumber, Word *wordPointer);
void freeUsedLabelList(UsedLabel *labels);
void freeUsedLabel(UsedLabel *label);

#endif
