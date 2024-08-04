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

#include "globals.h" /* Typedefs. */

/**
 * Links all the instances of used labels with their definitions.
 * Modifies the words in the code and data parts to include the addresses of the
 * labels.
 * Also, checks and returns whether or not all of the used labels are defined or
 * declared as extern.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the instruction count is the final instruction count of the file
 * to compile.
 *
 * @param fileName The name of the current file that is being compiled.
 * @param externLabels The list of extern labels.
 * @param usedLabels The list of used labels.
 * @param foundLabels The list of found labels.
 * @param instructionCount The final instruction count.
 * @return TRUE if no errors occurred (all labels are defined or declared as
 * extern), FALSE otherwise.
 */
Boolean linkLabels(char fileName[], Label *externLabels, UsedLabel *usedLabels,
                   FoundLabel *foundLabels, WordCount instructionCount);

#endif
