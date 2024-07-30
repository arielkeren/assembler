/*
 * labelLinking.h
 *
 * Contains the function prototypes for the functions in labelLinking.c.
 *
 * Name: Ariel Keren
 * Course: C Lab, 20465
 * Semester: 2024B
 * Date: 30/07/2024
 */

#ifndef LABEL_LINKING_H
#define LABEL_LINKING_H

#include <stdio.h>

#include "globals.h"

Boolean linkLabels(char fileName[], Label *externLabels, UsedLabel *usedLabels, FoundLabel *foundLabels, WordCount instructionCount);

#endif
