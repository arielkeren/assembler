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

void addLabel(Label **labels, char labelName[], LineNumber lineNumber);
Boolean containsLabel(Label *labels, char labelName[]);
Length getLongestLabel(Label *labels);
void freeLabelList(Label *labels);
void freeLabel(Label *label);

#endif
