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

#include "globals.h"

void freeAll();
void setToFree(Word **code, Word **data, Macro **macros, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels);
void freeingLogic(Boolean shouldFree, Word **code, Word **data, Macro **macros, Label **entryLabels, Label **externLabels, UsedLabel **usedLabels, FoundLabel **foundLabels);
void freeLists(ToFree *toFree);

#endif
