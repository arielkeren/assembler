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

/**
 * Handles an undefined label.
 * Returns whether the label has been declared as extern.
 * If so, sets the addressing mode of the target word to E.
 * If not, prints an error.
 *
 * Assumes that the given file name is not NULL and is null-terminated.
 * Assumes that the given used label is not NULL.
 *
 * @param fileName The name of the current file that is being compiled.
 * @param externLabels The list of extern labels.
 * @param usedLabel The undefined used label.
 * @return TRUE if the label has been declared as extern, FALSE otherwise.
 */
Boolean handleUndefinedLabel(char fileName[], Label *externLabels,
                             UsedLabel *usedLabel);

/**
 * Handles a defined label.
 * Sets the addressing mode of the target word to R.
 * Encodes the label's definition's address into the target word, based on
 * whether the label is defined in the data part or in the code part.
 *
 * Assumes that the given used label is not NULL.
 * Assumes that the given found label is not NULL.
 *
 * @param usedLabel The defined used label.
 * @param foundLabel The matching found label.
 * @param instructionCount The final instruction count.
 */
void handleDefinedLabel(UsedLabel *usedLabel, FoundLabel *foundLabel,
                        WordCount instructionCount);

#endif
