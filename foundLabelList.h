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

void addFoundLabel(FoundLabel **labels, char labelName[]);
void markAsData(FoundLabel *label);
void setAddress(FoundLabel *label, Address newAddress);
FoundLabel *getFoundLabel(FoundLabel *labels, char labelName[]);
void freeFoundLabelList(FoundLabel *labels);
void freeFoundLabel(FoundLabel *label);

#endif
